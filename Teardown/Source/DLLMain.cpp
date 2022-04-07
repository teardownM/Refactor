#include "Functions.h"
#include "Signatures.h"
#include "Globals.h"
#include "Console.h"
#include "detours.h"
#include <filesystem>
#include "Teardown/Menu.h"

__declspec(dllexport) int __stdcall _(void) { return 0; }

void Shutdown(HMODULE hModule, DWORD reason) {
    if (!hModule)
        FreeLibraryAndExitThread(GetModuleHandle(nullptr), reason);

    Teardown::Menu::Revert();
#ifdef DEBUG
    ::Utilities::Console::Close();
#endif
    FreeLibraryAndExitThread(hModule, reason);
}

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
    bool bRunning = true;

    g_Module = GetModuleHandle(nullptr);

    Teardown::Path = std::filesystem::current_path().string();
    if (Teardown::Menu::Set() != 0) {
        LOG_ERROR("Failed to create menu");
        Shutdown(g_Module, 1);
        return 1;
    }

/*    LOG_DEBUG("Getting Addresses...");
    Teardown::GetFunctionAddresses();*/

//
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ulReason, [[maybe_unused]] LPVOID lpReserved) {
    switch (ulReason) {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hModule);
            CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(StartRoutine), hModule, 0, nullptr);
            break;
        case DLL_PROCESS_DETACH: {
            Shutdown(g_Module, 0);
            break;
        } default: break;
    }

    return TRUE;
}