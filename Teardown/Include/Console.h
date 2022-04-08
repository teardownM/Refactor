#pragma once

#include "Logger.h"

namespace Utilities::Console {
    static FILE *console;

    static void Initialize() {
        AllocConsole();

        freopen_s(&console, "CONOUT$", "w", stdout);
        freopen_s(&console, "CONOUT$", "w", stderr);
        freopen_s(&console, "CONIN$", "r", stdin);

        SetConsoleTitleA("Teardown Multiplayer | DEBUG");
    }

    static void Close() {
        fclose(console);
        FreeConsole();
    }
}