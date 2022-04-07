#pragma once

#include "Memory.h"

namespace Signatures {
	/*
		memory sigs
	*/
	inline CSignature free("\xE8\x00\x00\x00\x00\x4C\x39\x26", "x????xxx");
	inline CSignature alloc("\xE8\x00\x00\x00\x00\xEB\x1F\x48\x85\xDB", "x????xxxxx");
	inline CSignature calloc("\xE8\x00\x00\x00\x00\x33\xC9\x48\x8B\xE8", "x????xxxxx");
	inline CSignature realloc("\xE8\x00\x00\x00\x00\x48\x8B\xF0\x48\x85\xC0\x74\x16", "x????xxxxxxxx");
	inline CSignature memmove("\x4C\x8B\xD9\x4C\x8B\xD2", "xxxxxx", false);

	/*
		sigs used for callbacks
	*/
	inline CSignature Update("\xE8\x00\x00\x00\x00\x48\x8D\x4D\xA7\xE8\x00\x00\x00\x00\xEB\x1E", "x????xxxxx????xx");
	inline CSignature SpawnPlayer("\xE8\x00\x00\x00\x00\xC6\x86\x00\x00\x00\x00\x00\x48\x8B\xCE", "x????xx?????xxx");
	inline CSignature UpdatePlayer("\xE8\x00\x00\x00\x00\x48\x8B\x4B\x78\xF3\x0F\x10\x8B\x00\x00\x00\x00", "x????xxxxxxxx????");
	inline CSignature SetState("\xE8\x00\x00\x00\x00\x44\x89\x76\x0C", "x????xxxx");
	inline CSignature Tick("\xE8\x00\x00\x00\x00\x89\xAB\x00\x00\x00\x00\x48\x8D\x4C\x24\x00", "x????xx????xxxx?");

	/*
		class constructors
	*/
	inline CSignature Game("\xE8\x00\x00\x00\x00\x48\x8B\xF8\xEB\x02\x33\xFF\x48\x8D\x4D\xA7", "x????xxxxxxxxxxx");

	inline CSignature Body("\xE8\x00\x00\x00\x00\x48\x8B\xF8\x33\xDB\xEB\x04", "x????xxxxxxx");
	inline CSignature Shape("\x33\xC9\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x03", "xxxxx????xxx");
	inline CSignature Light("\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x07\xC6\x47\x28\x01", "xxx????xxxxxxx");
	inline CSignature Location("\xE8\x00\x00\x00\x00\x44\x8B\xCF\x48\x89\x7C\x24\x00", "x????xxxxxxx?");
	inline CSignature Water("\x48\x8D\x05\x00\x00\x00\x00\x49\x89\x06\x33\xFF", "xxx????xxxxx");
	inline CSignature Enemy("\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x03\x48\x89\x7B\x28", "xxx????xxxxxxx");
	inline CSignature Joint("\xC7\x43\x00\x00\x00\x00\x00\x48\x8D\x05\x00\x00\x00\x00", "xx?????xxx????");
	inline CSignature Vehicle("\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x03\x33\xFF", "xxx????xxxxx");
	inline CSignature Wheel("\xE8\x00\x00\x00\x00\x44\x8B\xCF\x48\x89\x7C\x24\x00", "x????xxxxxxx?");
	inline CSignature Screen("\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x07\x33\xF6\x48\x89\x77\x28", "xxx????xxxxxxxxx");
	inline CSignature Trigger("\xE8\x00\x00\x00\x00\x44\x8B\xCF\x48\x89\x7C\x24\x00", "x????xxxxxxx?");
	inline CSignature Script("\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x01\x33\xFF", "xxx????xxxxx");

	/*
		entity sigs
	*/
	inline CSignature GenVoxTexture("\xE8\x00\x00\x00\x00\x41\x0F\x10\x46\x00", "x????xxxx?");
	inline CSignature GenVoxPhysBuffer("\xE8\x00\x00\x00\x00\x41\xFF\xC7\x48\x83\xC7\x1C", "x????xxxxxxx");
	inline CSignature LoadVox("\xE8\x00\x00\x00\x00\x49\x89\x86\x00\x00\x00\x00", "x????xxx????");

	inline CSignature InitializeBody("\xE8\x00\x00\x00\x00\x48\x85\xF6\x74\x2B", "x????xxxxx");
	inline CSignature UpdateBodyDynamic("\xE8\x00\x00\x00\x00\x4C\x8D\x9C\x24\x00\x00\x00\x00\x49\x8B\x73\x30", "x????xxxx????xxxx");
	inline CSignature UnknownBodyFunction("\x48\x8B\xC4\x44\x88\x40\x18\x55", "xxxxxxxx", true);

	/*
		registry sigs
	*/
	inline CSignature SetBool("\xE8\x00\x00\x00\x00\x90\x48\x8D\x4D\x87\xE8\x00\x00\x00\x00\x84\xDB", "x????xxxxxx????xx");
	inline CSignature SetInt("\xE8\x00\x00\x00\x00\x90\x48\x8D\x4D\xE8\xE8\x00\x00\x00\x00\x33\xD2", "x????xxxxxx????xx");
	inline CSignature SetFloat("\xE8\x00\x00\x00\x00\x48\x8D\x4D\x50", "x????xxxx");
	inline CSignature SetString("\xE8\x00\x00\x00\x00\x90\xEB\x1F", "x????xxx");

	inline CSignature GetBool("\xE8\x00\x00\x00\x00\x84\xC0\x74\x05\x40\xB6\x01", "x????xxxxxxx");
	inline CSignature GetInt("\xE8\x00\x00\x00\x00\x89\x43\x2C", "x????xxx");
	inline CSignature GetFloat("\xE8\x00\x00\x00\x00\xF3\x0F\x59\xF8", "x????xxxx");
	inline CSignature GetString("\x40\x53\x48\x83\xEC\x30\x49\x8B\xC0", "xxxxxxxxx", false);

	/*
		tag sigs
	*/
	inline CSignature SetTag("\xE8\x00\x00\x00\x00\x88\x06", "x????xx", false);
	inline CSignature HasTag("\xE8\x00\x00\x00\x00\x0F\xB6\xD8\x48\x8D\x4D\x90", "x????xxxxxxx");
	inline CSignature RemoveTag("\x40\x53\x41\x56\x48\x83\xEC\x58\xF6\x41\x0A\x01", "xxxxxxxxxxxx", false);
	inline CSignature GetTag("\xE8\x00\x00\x00\x00\x90\x48\x8B\xC8\xE8\x00\x00\x00\x00\x41\x0F\x2E\xC2", "x????xxxxx????xxxx");

	/*
		misc sigs
	*/
	inline CSignature Log("\x80\x79\x0F\x00\x74\x03\x48\x8B\x09\x48\x8B\xD1\x48\x8D\x0D\x00\x00\x00\x00", "xxxxxxxxxxxxxxx????", false);
	inline CSignature IsWindowActive("\xE8\x00\x00\x00\x00\x84\xC0\x74\x16\x48\x8B\xD3", "x????xxxxxxx");
	inline CSignature SetCursorCaptured("\x48\x83\xEC\x28\x48\x8B\x41\x08\x38\x50\x23", "xxxxxxxxxxx", false);
	inline CSignature QueryRaycast("\xE8\x00\x00\x00\x00\x84\xC0\x75\x5A", "x????xxxx");
	inline CSignature DebugPrint("\x44\x8B\x81\x00\x00\x00\x00\x4C\x8B\xD2", "xxx????xxx", false);
	inline CSignature DrawLine("\xE8\x00\x00\x00\x00\xFF\xC3\x48\x83\xC6\x0C", "x????xxxxxx");
	inline CSignature SpawnParticle("\xE8\x00\x00\x00\x00\x8B\x75\xEC", "x????xxx");
	inline CSignature LoadSound("\xE8\x00\x00\x00\x00\x48\x89\x45\x27\x48\x8D\x4D\xEF\xE8\x00\x00\x00\x00\x90\xE9\x00\x00\x00\x00", "x????xxxxxxxxx????xx????");
	inline CSignature LoadLoop("\xE8\x00\x00\x00\x00\x48\x89\x87\x00\x00\x00\x00\x48\x8D\x4C\x24\x00", "x????xxx????xxxx?");
	inline CSignature PlaySound("\xE8\x00\x00\x00\x00\x41\x0F\x2F\xF5", "x????xxxx");
	inline CSignature PlayLoop("\xE8\x00\x00\x00\x00\x83\x7F\x48\x01", "x????xxxx");
	inline CSignature SpotLight("\xE8\x00\x00\x00\x00\x41\x0F\x28\xCF\x49\x8B\xCE", "x????xxxxxxx");
	inline CSignature CreateExplosion("\xE8\x00\x00\x00\x00\xE9\x00\x00\x00\x00\x44\x89\xB5\x00\x00\x00\x00", "x????x????xxx????");
	inline CSignature MakeHole("\xE8\x00\x00\x00\x00\x83\xFB\x05", "x????xxx");
	inline CSignature ApplyDisplay("\x89\x15\x00\x00\x00\x00\x44\x89\x05\x00\x00\x00\x00", "xx????xxx????", false);
}