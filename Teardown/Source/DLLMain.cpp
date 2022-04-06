#include "Functions.h"
#include "Signatures.h"
#include "Globals.h"
#include "Console.h"
#include "detours.h"

__declspec(dllexport) int __stdcall _(void) { return 0; }

DWORD WINAPI StartRoutine([[maybe_unused]] HMODULE hModule) {
#ifdef DEBUG
    ::Utilities::Console::Initialize();
#else
    #define LOG_TRACE(...)
    #define LOG_DEBUG(...)
    #define LOG_INFO(...)
    #define LOG_WARN(...)
    #define LOG_ERROR(...)
    #define LOG_FATAL(...)
#endif

    LOG_DEBUG("Starting...");
	
    g_Module = GetModuleHandle(nullptr);

    LOG_DEBUG("Getting Addresses...");
    Teardown::GetFunctionAddresses();

#ifdef DEBUG
    ::Utilities::Console::Close();
#endif

    FreeLibraryAndExitThread(hModule, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, [[maybe_unused]] LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CloseHandle(CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(StartRoutine), hModule, 0, nullptr));
        break;
    case DLL_PROCESS_DETACH:
        break;
    default:
        break;
    }

    return TRUE;
}