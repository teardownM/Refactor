#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "detours.h"
#include <filesystem>
#include <TlHelp32.h>
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <chrono>
#include <thread>

const char* GetGamePath() {
    char cSteamPath[MAX_PATH];
	HKEY SteamKey;

	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\WOW6432Node\\Valve\\Steam", 0, KEY_QUERY_VALUE, &SteamKey) == ERROR_SUCCESS) {
		DWORD dwLen = MAX_PATH;
		if (RegQueryValueExA(SteamKey, "InstallPath", 0, 0, reinterpret_cast<LPBYTE>(&cSteamPath), &dwLen) == ERROR_SUCCESS) {
			cSteamPath[dwLen - 1] = '\0';
        } else {
			return 0;
        }

		RegCloseKey(SteamKey);
	} else {
		return 0;
    }

    std::string sSteamPath = std::string(cSteamPath);
	if (sSteamPath.empty())
		return 0;

    char* cTeardownPath = new char[MAX_PATH];

	std::string sTeardownPath = sSteamPath + "\\steamapps\\common\\Teardown";
	if (std::filesystem::exists(sTeardownPath + "\\teardown.exe")) {
		memcpy(cTeardownPath, sTeardownPath.c_str(), MAX_PATH);
		return cTeardownPath;
	}

    // Look at all the other steam directories for the game
    std::ifstream ConfigFile(sSteamPath + "\\steamapps\\libraryfolders.vdf");
	if (!ConfigFile.is_open()) {
        std::cerr << "Failed to open libraryfolders.vdf!" << std::endl;
		return 0;
    }

	std::string sConfigContent = std::string(std::istreambuf_iterator<char>(ConfigFile), std::istreambuf_iterator<char>());
	std::regex DirRegex("\"[^\"]+\"[\\s]+\"([^\"]+)\"\\n", std::regex::ECMAScript);

    std::regex_iterator LibraryFolders = std::sregex_iterator(sConfigContent.begin(), sConfigContent.end(), DirRegex);

	for (std::sregex_iterator Match = LibraryFolders; Match != std::sregex_iterator(); ++Match) {
		sTeardownPath = (*Match)[1].str() + "\\steamapps\\common\\Teardown";

		if (std::filesystem::exists(sTeardownPath)) {
			sTeardownPath.replace(sTeardownPath.find("\\\\"), 2, "\\");

			if (std::filesystem::exists(sTeardownPath + "\\teardown.exe")) {
				memcpy(cTeardownPath, sTeardownPath.c_str(), MAX_PATH);
				return cTeardownPath;
			}
		}
	}

    return 0;
}

DWORD GetPIDByName(const std::wstring& name) {
    PROCESSENTRY32 pt;
    HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    pt.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hsnap, &pt)) {
        do {
            if (!lstrcmpi(pt.szExeFile, name.c_str())) {
                CloseHandle(hsnap);
                return pt.th32ProcessID;
            }
        } while (Process32Next(hsnap, &pt));
    }

    CloseHandle(hsnap);
    return 0;
};

int LaunchGame(PROCESS_INFORMATION* ProcInfo, const char* cExePath, const char* cTeardownPath) {
	STARTUPINFOA StartupInfo;
	ZeroMemory(&StartupInfo, sizeof(StartupInfo));

	if (!CreateProcessA(nullptr, const_cast<LPSTR>(cExePath), nullptr, nullptr, TRUE, CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED, nullptr, cTeardownPath, &StartupInfo, ProcInfo)) {
        DWORD dwError = GetLastError();
        LPVOID lpMsgBuf;
        FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr,
            dwError,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPSTR)&lpMsgBuf,
            0, nullptr);
        MessageBoxA(nullptr, (LPCSTR)lpMsgBuf, "Error Starting Game", MB_ICONERROR | MB_OK);
		return 1;
	}

    return 0;
}

int main() {
    const char* cTeardownPath = GetGamePath();
    char cDLLPath[MAX_PATH];
    char cCurrentPath[MAX_PATH];
    char cExePath[MAX_PATH];

    PROCESS_INFORMATION ProcInfo;
    STARTUPINFOA StartupInfo;

    ZeroMemory(&ProcInfo, sizeof(ProcInfo));
    ZeroMemory(&StartupInfo, sizeof(StartupInfo));

    if (!cTeardownPath) {
        MessageBoxA(nullptr, "Unable to find Teardown installation", "Error", MB_ICONERROR | MB_OK);
        return 1;
    }

    GetCurrentDirectoryA(MAX_PATH, cCurrentPath);
    sprintf_s(cDLLPath, "%s\\%s", cCurrentPath, "Teardown.dll");
    const char* cDLLPath2 = cDLLPath;

    if (!std::filesystem::exists(cDLLPath)) {
        MessageBoxA(nullptr, "Unable to find Teardown.dll", "Error", MB_ICONERROR | MB_OK);
        return 1;
    }

    sprintf_s(cExePath, "%s\\%s", cTeardownPath, "teardown.exe");
    if (!std::filesystem::exists(cExePath)) {
        MessageBoxA(nullptr, "Unable to find teardown.exe", "Error", MB_ICONERROR | MB_OK);
        return 1;
    }

    FILE* TeardownExe;
    fopen_s(&TeardownExe, cExePath, "rb");
    if (TeardownExe == nullptr) {
        MessageBoxA(nullptr, "Unable to open teardown.exe", "Error", MB_ICONERROR | MB_OK);
        return 1;
    }

    fseek(TeardownExe, 0, SEEK_END);
    long lFileSize = ftell(TeardownExe);
    rewind(TeardownExe);

    void* pExeBuffer = malloc(lFileSize);
    if (pExeBuffer == nullptr) {
        MessageBoxA(nullptr, "Unable to allocate memory for exe buffer", "Error", MB_ICONERROR | MB_OK);
        return 1;
    }

    fread(pExeBuffer, lFileSize, 1, TeardownExe);
    fclose(TeardownExe);

    SetEnvironmentVariableA("SteamAppId", "1167630"); // Set SteamAppId var to initialize SteamAPI

    const DWORD PID = GetPIDByName(L"Teardown.exe");

    if (PID == 0) {
        // Launch the game
        std::cout << "Launching Game\n";
        LaunchGame(&ProcInfo, cExePath, cTeardownPath);
    } else {
        // Attach to the game
    	ProcInfo.hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
        ProcInfo.dwProcessId = PID;
    }

    if (!ProcInfo.hProcess) {
        MessageBoxA(nullptr, "Unable to launch game", "Error", MB_ICONERROR | MB_OK);
        return 1;
    }

    const size_t dwDLLPath2Length = strlen(cDLLPath2);

    // Allocate memory for the DLL
    const LPVOID pRemoteDLL = VirtualAllocEx(ProcInfo.hProcess, nullptr, dwDLLPath2Length + 1, MEM_COMMIT, PAGE_READWRITE);
    if (!pRemoteDLL) {
        const DWORD dwError = GetLastError();
        LPVOID lpMsgBuf;
        FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr,
            dwError,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            reinterpret_cast<LPSTR>(&lpMsgBuf),
            0, nullptr);
        MessageBoxA(nullptr, static_cast<LPCSTR>(lpMsgBuf), "VirtualAllocEx Failed", MB_ICONERROR | MB_OK);
        return 1;
    }

    // Write the DLL to the process
    if (!WriteProcessMemory(ProcInfo.hProcess, pRemoteDLL, cDLLPath2, dwDLLPath2Length + 1, nullptr)) {
        // get last error
        const DWORD dwError = GetLastError();
        LPVOID lpMsgBuf;
        FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr,
            dwError,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            reinterpret_cast<LPSTR>(&lpMsgBuf),
            0, nullptr);
        MessageBoxA(nullptr, static_cast<LPCSTR>(lpMsgBuf), "WriteProcessMemory Failed", MB_ICONERROR | MB_OK);
        return 1;
    }

    // Get the address of LoadLibraryA
    const auto pLoadLibraryA = reinterpret_cast<LPVOID>(GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA"));
    if (!pLoadLibraryA) {
        MessageBoxA(nullptr, "GetProcAddress failed", "Error", MB_ICONERROR | MB_OK);
        return 1;
    }

    if (!ProcInfo.hThread) {
        ProcInfo.hThread = CreateRemoteThread(ProcInfo.hProcess, nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(pLoadLibraryA), pRemoteDLL, 0, nullptr);
    } else {
        CreateRemoteThread(ProcInfo.hProcess, nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(pLoadLibraryA), pRemoteDLL, 0, nullptr);
    }

    if (!ProcInfo.hThread) {
        MessageBoxA(nullptr, "CreateRemoteThread failed", "Error", MB_ICONERROR | MB_OK);
        return 1;
    }

    ResumeThread(ProcInfo.hThread);

	CloseHandle(ProcInfo.hProcess);
	CloseHandle(ProcInfo.hThread);

    return 0;
}