#pragma once

#include "Logger.h"

namespace Utilities {
    namespace Console {
        static FILE* console;

        static void Initialize() {
            AllocConsole();
        
            freopen_s(&console, "CONOUT$", "w", stdout);
            freopen_s(&console, "CONOUT$", "w", stderr);
            freopen_s(&console, "CONIN$", "r", stdin);

            SetConsoleTitleA("Teardown Multiplayer | DEBUG");
            Logger::Initialize();
        }

        static void Close() {
            fclose(console);
            FreeConsole();
        }
    }
}