#pragma once
#include "Teardown/Script.h"
#include "Teardown/Entities.h"

namespace Teardown::Constructors {
    typedef void *(*tClassConstructor)(void *pAlloc, void *pParent);
	typedef void (*tRegisterGameFunctions)		(CScriptCore* pScriptCore);
	typedef void (*tRegisterLuaFunction)		(CScriptCore_LuaState* pSCLS, small_string* sFunctionName, void* pFunction);

    inline tClassConstructor tdScript;
	inline tRegisterGameFunctions tdRegisterGameFunctions;
	inline tRegisterLuaFunction tdRegisterLuaFunction;

	inline void RegisterLuaFunction(CScriptCore_LuaState* pSCLS, const char* cFunctionName, void* pFunction)
	{
		small_string sFuncName(cFunctionName);
		tdRegisterLuaFunction(pSCLS, &sFuncName, pFunction);
	}

	inline Script* CreateScript(Entity* pParent)
	{
		void* pAlloc = Teardown::MemoryFunctions::Alloc(sizeof(Script));
		return (Script*)Teardown::Constructors::tdScript(pAlloc, pParent);
	}
}