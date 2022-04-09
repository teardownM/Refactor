#pragma once

#include "Memory.h"

namespace Signatures {
    /////////////////////////
    //     Memory Sigs     //
    /////////////////////////
    inline CSignature malloc_base("\xE8\x00\x00\x00\x00\xEB\x1F\x48\x85\xDB", "x????xxxxx", true); // Outdated (v0.9.2)
    inline CSignature free_base("\xE8\x00\x00\x00\x00\xEB\x1F\x48\x85\xDB", "x????xxxxx", true); // Outdated (v0.9.2)

    ///////////////////////////
    //     Lua Functions     //
    ///////////////////////////
    inline CSignature RegisterGameFunctions("\x48\x89\x5C\x24\x00\x57\x48\x83\xEC\x30\x48\x8D\x79\x30", "xxxx?xxxxxxxxx", false);
    inline CSignature RegisterLuaFunctionSC("\x48\x89\x5C\x24\x00\x57\x48\x83\xEC\x30\x48\x8D\x79\x30", "xxxx?xxxxxxxxx");
    //inline CSignature luaL_loadbuffer("\xE8\x00\x00\x00\x00\x85\xC0\x75\x07\xB8\x00\x00\x00\x00\xEB\x57", "x????xxxxx????xx", true); // Outdated (v0.9.2)

    /////////////////////////
    ///     Callbacks      //
    /////////////////////////
	inline CSignature Update("\xE8\x00\x00\x00\x00\x48\x8D\x4D\xA7\xE8\x00\x00\x00\x00\xEB\x1E", "x????xxxxx????xx");
	inline CSignature SpawnPlayer("\xE8\x00\x00\x00\x00\xC6\x86\x00\x00\x00\x00\x00\x48\x8B\xCE", "x????xx?????xxx");
	inline CSignature UpdatePlayer("\xE8\x00\x00\x00\x00\x48\x8B\x4B\x78\xF3\x0F\x10\x8B\x00\x00\x00\x00", "x????xxxxxxxx????");
	inline CSignature SetState("\xE8\x00\x00\x00\x00\x44\x89\x76\x0C", "x????xxxx");
	inline CSignature Tick("\xE8\x00\x00\x00\x00\x89\xAB\x00\x00\x00\x00\x48\x8D\x4C\x24\x00", "x????xx????xxxx?");

    /////////////////////////
    //     Class CTORs     //
    /////////////////////////
	inline CSignature Game("\xE8\x00\x00\x00\x00\x48\x8B\xF8\xEB\x02\x33\xFF\x48\x8D\x4D\xA7", "x????xxxxxxxxxxx");
	inline CSignature Script("\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x01\x33\xFF", "xxx????xxxxx");

    /////////////////////////
    //        Other        //
    /////////////////////////
	inline CSignature IsWindowActive("\xE8\x00\x00\x00\x00\x84\xC0\x74\x16\x48\x8B\xD3", "x????xxxxxxx");
}