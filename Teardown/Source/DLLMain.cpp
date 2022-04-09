#include <filesystem>

#include "Globals.h"
#include "Console.h"

#include "Launcher.h"
#include "Teardown/Functions.h"
#include "Teardown/Menu.h"

#include <libloaderapi.h>

#define DLL_EXPORT extern "C" __declspec(dllexport)

[[maybe_unused]] __declspec(dllexport) int __stdcall _() { return 0; }

DLL_EXPORT int ReloadUI() {
    LOG_INFO("Reloading UI");
    ExitThread(0);
}

///////////////////////////////////
///////     Functions     /////////
///////////////////////////////////
void Shutdown(HMODULE hModule, DWORD reason) {
    Teardown::Menu::Revert();
    LOG_INFO("Goodbye :) Exit code: {}", reason);

#ifdef DEBUG
    ::Utilities::Console::Close();
#endif
    FreeLibraryAndExitThread(hModule, reason);
}

DWORD WINAPI StartRoutine(HMODULE hModule) {
    g_Module = GetModuleHandle(nullptr);

#ifdef _DEBUG
    ::Utilities::Console::Initialize();
#endif

    if (!Launcher::GetPath()) {
        Shutdown(reinterpret_cast<HMODULE>(hModule), 1);
    }

    Logger::Initialize();

    Teardown::Path = std::filesystem::current_path().string();
    if (Teardown::Menu::Set() != 0) {
        LOG_ERROR("Failed to create menu");
        Shutdown(reinterpret_cast<HMODULE>(hModule), 1);
    }

    LOG_INFO("Successfully Loaded Teardown Multiplayer");

    Teardown::GetFunctionAddresses();
    return 0;
}

///////////////////////////////////
///////     Entry Point     ///////
///////////////////////////////////
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ulReason, [[maybe_unused]] LPVOID lpReserved) {
    switch (ulReason) {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hModule);
            CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(StartRoutine), hModule, 0, nullptr);
            break;
        case DLL_PROCESS_DETACH:
            Shutdown(reinterpret_cast<HMODULE>(hModule), 0);
            break;
        default:
            break;
    };

    return TRUE;
}