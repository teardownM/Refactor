#include "Memory.h"
#include "Globals.h"

#include <minwindef.h>
#include <Psapi.h>
#include <processthreadsapi.h>

#include <mutex>


DWORD64 dwBaseAddress;
DWORD dwModuleSize;

std::once_flag fHasModuleInfo;

bool bPatternCompare(const char* cData, const char* cPattern, const char* cMask) {
	for (; *cMask; ++cMask, ++cData, ++cPattern) {
		if (*cMask == 'x' && *cData != *cPattern)
			return false;
	}
	return true;
}

void GetModuleInfo() {
	MODULEINFO ModuleInfo = { nullptr };
	GetModuleInformation(GetCurrentProcess(), static_cast<HMODULE>(g_Module), &ModuleInfo, sizeof(MODULEINFO));

	dwBaseAddress = reinterpret_cast<DWORD64>(g_Module);
	dwModuleSize = ModuleInfo.SizeOfImage;
}

unsigned __int64 Memory::dwReadPtr(DWORD64 dwAddress, UINT iOffset) {
	const int iRelOffset = *(int*)(dwAddress + iOffset);
	return dwAddress + iRelOffset + sizeof(int) + iOffset;
}

unsigned __int64 Memory::dwFindPattern(CSignature Sig) {
	std::call_once(fHasModuleInfo, GetModuleInfo);

	for (unsigned __int64 i = 0; i < dwModuleSize; i++) {
		if (bPatternCompare(reinterpret_cast<char*>(dwBaseAddress + i), Sig.m_Pattern, Sig.m_Mask)) {
			if (Sig.m_DirectRef)
				return Memory::dwReadPtr(dwBaseAddress + i, 1);
			return dwBaseAddress + i;
		}
	}

	return 0;
}