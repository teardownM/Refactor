#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <filesystem>
#include <TlHelp32.h>
#include <iostream>
#include <fstream>
#include <string>
#include <regex>

#include "spdlog/spdlog.h"

const char *GetGamePath() {
    char cSteamPath[MAX_PATH];
    HKEY SteamKey;

    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, R"(SOFTWARE\WOW6432Node\Valve\Steam)", 0, KEY_QUERY_VALUE, &SteamKey) ==
        ERROR_SUCCESS) {
        DWORD dwLen = MAX_PATH;
        if (RegQueryValueExA(SteamKey, "InstallPath", nullptr, nullptr, reinterpret_cast<LPBYTE>(&cSteamPath),
                             &dwLen) == ERROR_SUCCESS) {
            cSteamPath[dwLen - 1] = '\0';
        } else {
            return nullptr;
        }

        RegCloseKey(SteamKey);
    } else {
        return nullptr;
    }

    std::string sSteamPath = std::string(cSteamPath);
    if (sSteamPath.empty())
        return nullptr;

    char *cTeardownPath = new char[MAX_PATH];

    std::string sTeardownPath = sSteamPath + R"(\steamapps\common\Teardown)";
    if (std::filesystem::exists(sTeardownPath + "\\teardown.exe")) {
        memcpy(cTeardownPath, sTeardownPath.c_str(), MAX_PATH);
        return cTeardownPath;
    }

    // Look at all the other steam directories for the game
    std::ifstream ConfigFile(sSteamPath + "\\steamapps\\libraryfolders.vdf");
    if (!ConfigFile.is_open()) {
        std::cerr << "Failed to open libraryfolders.vdf!" << std::endl;
        return nullptr;
    }

    std::string sConfigContent = std::string(std::istreambuf_iterator<char>(ConfigFile),
                                             std::istreambuf_iterator<char>());
    std::regex DirRegex("\"[^\"]+\"[\\s]+\"([^\"]+)\"\\n", std::regex::ECMAScript);

    std::regex_iterator LibraryFolders = std::sregex_iterator(sConfigContent.begin(), sConfigContent.end(), DirRegex);

    for (std::sregex_iterator Match = LibraryFolders; Match != std::sregex_iterator(); ++Match) {
        sTeardownPath = (*Match)[1].str() + R"(\steamapps\common\Teardown)";

        if (std::filesystem::exists(sTeardownPath)) {
            sTeardownPath.replace(sTeardownPath.find("\\\\"), 2, "\\");

            if (std::filesystem::exists(sTeardownPath + "\\teardown.exe")) {
                memcpy(cTeardownPath, sTeardownPath.c_str(), MAX_PATH);
                return cTeardownPath;
            }
        }
    }

    return nullptr;
}

DWORD GetPIDByName(const std::wstring &name) {
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

void Shutdown(const std::string &message, int exitCode) {
    spdlog::error(message + "\nPress any key to continue");
    std::cin.get();
    exit(exitCode);
}

void ShutdownLastError(const std::string &message) {
    DWORD dwError = GetLastError();
    LPVOID lpMsgBuf;
    FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr,
            dwError,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPSTR) &lpMsgBuf,
            0, nullptr);
    Shutdown(message + ": " + (LPCSTR) lpMsgBuf, 1);
}

int LaunchGame(PROCESS_INFORMATION *ProcInfo, const char *cExePath, const char *cTeardownPath) {
    STARTUPINFOA StartupInfo;
    ZeroMemory(&StartupInfo, sizeof(StartupInfo));

    if (!CreateProcessA(nullptr, const_cast<LPSTR>(cExePath), nullptr, nullptr, TRUE,
                        CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED, nullptr, cTeardownPath, &StartupInfo, ProcInfo)) {
        ShutdownLastError("CreateProcessA Failed");
        return 1;
    }

    return 0;
}

int main() {
#ifdef _DEBUG
    spdlog::set_level(spdlog::level::debug);
#else
    spdlog::set_level(spdlog::level::info);
#endif

    const char *cTeardownPath = GetGamePath();
    char cDLLPath[MAX_PATH];
    char cCurrentPath[MAX_PATH];
    char cExePath[MAX_PATH];

    PROCESS_INFORMATION ProcInfo;
    STARTUPINFOA StartupInfo;

    ZeroMemory(&ProcInfo, sizeof(ProcInfo));
    ZeroMemory(&StartupInfo, sizeof(StartupInfo));

    if (!cTeardownPath) Shutdown("Unable to find installation of teardown", 1);

    GetCurrentDirectoryA(MAX_PATH, cCurrentPath);
    sprintf_s(cDLLPath, "%s\\%s", cCurrentPath, "Teardown.dll");
    const char *cDLLPath2 = cDLLPath;

    char cTempPath[MAX_PATH];
    GetTempPathA(MAX_PATH, cTempPath);
    sprintf_s(cTempPath, "%s\\%s", cTempPath, "tdl.txt");

    std::ofstream TempFile(cTempPath);
    TempFile << cCurrentPath;
    TempFile.close();

    spdlog::debug("Wrote tdl.txt to {}", cTempPath);

    spdlog::debug("DLL Path: {}", cDLLPath2);
    spdlog::debug("Current Path: {}", cCurrentPath);

    if (!std::filesystem::exists(cDLLPath)) Shutdown("Unable to find Teardown.dll", 1);

    sprintf_s(cExePath, "%s\\%s", cTeardownPath, "teardown.exe");
    if (!std::filesystem::exists(cExePath)) Shutdown("Unable to find installation of teardown", 1);

    spdlog::debug("ExePath: {}", cExePath);
    spdlog::debug("Teardown Path: {}", cTeardownPath);

    FILE *TeardownExe;
    fopen_s(&TeardownExe, cExePath, "rb");
    if (!TeardownExe) Shutdown("Failed opening Teardown", 1);

    fseek(TeardownExe, 0, SEEK_END);
    long lFileSize = ftell(TeardownExe);
    rewind(TeardownExe);

    void *pExeBuffer = malloc(lFileSize);
    if (!pExeBuffer) Shutdown("Failed Getting Teardown Filesize", 1);

    fread(pExeBuffer, lFileSize, 1, TeardownExe);
    fclose(TeardownExe);

    SetEnvironmentVariableA("SteamAppId", "1167630"); // Set SteamAppId var to initialize SteamAPI

    const DWORD PID = GetPIDByName(L"Teardown.exe");
    bool bManuallyLaunched = false;

    if (PID == 0) {
        // Launch the game
        spdlog::info("Launching Teardown");
        LaunchGame(&ProcInfo, cExePath, cTeardownPath);
    } else {
        // Attach to the game
        spdlog::info("Attaching to Teardown");
        ProcInfo.hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
        ProcInfo.dwProcessId = PID;
        spdlog::debug("PID: {}", PID);
        bManuallyLaunched = true;
    }

    spdlog::debug("hProcess: {:p}", ProcInfo.hProcess);

    if (!ProcInfo.hProcess) Shutdown("Failed launching/attaching to Teardown", 1);

    const size_t dwDLLPath2Length = strlen(cDLLPath2);

    // Allocate memory for the DLL
    const LPVOID pRemoteDLL = VirtualAllocEx(ProcInfo.hProcess, nullptr, dwDLLPath2Length + 1, MEM_COMMIT,
                                             PAGE_READWRITE);
    spdlog::debug("Allocated {} bytes for DLL", dwDLLPath2Length + 1);
    spdlog::debug("pRemoteDLL: {:p}", pRemoteDLL);
    if (!pRemoteDLL) {
        ShutdownLastError("VirtualAllocEx Failed");
        return 1;
    }

    // Write the DLL to the process
    if (!WriteProcessMemory(ProcInfo.hProcess, pRemoteDLL, cDLLPath2, dwDLLPath2Length + 1, nullptr)) {
        ShutdownLastError("WriteProcessMemory Failed");
        return 1;
    }

    // Get the address of LoadLibraryA
    const auto pLoadLibraryA = reinterpret_cast<LPVOID>(GetProcAddress(GetModuleHandleA("kernel32.dll"),
                                                                       "LoadLibraryA"));
    spdlog::debug("pLoadLibraryA: {:p}", pLoadLibraryA);
    if (!pLoadLibraryA) {
        ShutdownLastError("GetProcAddress Failed");
        return 1;
    }

    if (!ProcInfo.hThread) {
        ProcInfo.hThread = CreateRemoteThread(ProcInfo.hProcess, nullptr, 0,
                                              reinterpret_cast<LPTHREAD_START_ROUTINE>(pLoadLibraryA), pRemoteDLL, 0,
                                              nullptr);
        ProcInfo.dwThreadId = GetThreadId(ProcInfo.hThread);
        spdlog::warn("The UI will not get reloaded (will add in the future)");
    } else {
        CreateRemoteThread(ProcInfo.hProcess, nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(pLoadLibraryA),
                           pRemoteDLL, 0, nullptr);
    }

    spdlog::debug("RemoteThread: {:p}", ProcInfo.hThread);

    if (!ProcInfo.hThread) {
        CloseHandle(ProcInfo.hProcess);
        ShutdownLastError("CreateRemoteThread Failed");
    }

    ResumeThread(ProcInfo.hThread);

    Sleep(2000);
    spdlog::info("Teardown Multiplayer has been Loaded! Have fun");

    // TODO: Reload UI if bManuallyLaunched

    WaitForSingleObject(ProcInfo.hProcess, INFINITE);

    CloseHandle(ProcInfo.hProcess);
    CloseHandle(ProcInfo.hThread);

    return 0;
}