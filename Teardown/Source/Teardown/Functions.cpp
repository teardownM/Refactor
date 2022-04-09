#include "Logger.h"
#include "Memory.h"

#include "Teardown/Functions.h"
#include "Teardown/Signatures.h"
#include "Teardown/Constructors.h"
#include "Teardown/Script.h"

struct SScannable {
	CSignature m_Sig;
	void* m_Function;
	const char* m_Name;
};

static const SScannable aFunctions[] = {
        { Signatures::Script, &Teardown::Constructors::tdScript, "CScript::CScript" },
        { Signatures::RegisterGameFunctions, &Teardown::Constructors::tdRegisterGameFunctions, "CScript::CScript" },
        { Signatures::RegisterLuaFunctionSC, &Teardown::Constructors::tdRegisterLuaFunction, "CScript::CScript" },
};

void Teardown::GetFunctionAddresses() {
    LOG_DEBUG("Getting Function Addresses");

	for (const SScannable& sScannable : aFunctions) {
		unsigned long long dwFuncAddr = Memory::dwFindPattern(sScannable.m_Sig);
        const void* vAddr = reinterpret_cast<void*>(dwFuncAddr);

        if (!vAddr) {
            LOG_ERROR("Function [{}] is Invalid!", sScannable.m_Name);
        } else {
            memcpy(sScannable.m_Function, &dwFuncAddr, sizeof(void *));
            LOG_DEBUG("{} - {}", sScannable.m_Name, vAddr);
        }
	}
}