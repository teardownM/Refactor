#include "LuaFunctions.h"
#include "lua.h"
#include "lauxlib.h"
#include <Logger.h>
#include "Teardown/Constructors.h"
#include "Teardown/CLuaFunctions.h"

bool bScriptHasFunction(lua_State* L, const char* funcName)
{
	lua_getglobal(L, "_G");
	lua_getfield(L, -1, funcName);
	bool isNil = lua_isnil(L, -1);

	lua_pop(L, 2);
	return !isNil;
}

void TDU::Lua::RunScript(std::string sScript)
{
	// Runing empty scripts causes heap corruptions, oddly enough
	if (sScript.length() == 0)
		return;

	Script* pScript = Teardown::Constructors::CreateScript(0);

	lua_State* L = pScript->m_ScriptCore.m_SCLuaState.m_LuaStateInfo->m_LuaState;

	Teardown::Constructors::tdRegisterGameFunctions(&pScript->m_ScriptCore);
	TDU::CLua::RegisterCFunctions(&pScript->m_ScriptCore.m_SCLuaState);

	if (luaL_loadbuffer(L, sScript.c_str(), sScript.length(), "TDU script") || lua_pcall(L, 0, LUA_MULTRET, 0))
	{
		const char* lastError = lua_tostring(L, -1);
		LOG_ERROR("Script error: %s", lastError);
		lua_pop(L, 2);
	}

	lua_pop(L, 1);

	pScript->m_ScriptCore.m_HasTick = bScriptHasFunction(L, "tick");
	pScript->m_ScriptCore.m_HasInit = bScriptHasFunction(L, "init");
	pScript->m_ScriptCore.m_HasUpdate = bScriptHasFunction(L, "update");

	/*
		UI Functions are currently not available, as that'd require reversing the HUD class, initializing it, setting the pointer at ScriptCore + 0x1C0 then calling RegisterUIFunctions
		once that's done, uncomment this
		(There may also be more steps to this)

		newScript->scriptCore.hasDraw = hasFunction(L, "draw");
	*/

	if (pScript->m_ScriptCore.m_HasTick || pScript->m_ScriptCore.m_HasInit || pScript->m_ScriptCore.m_HasUpdate)
		pScript->m_ScriptCore.m_HasCallbacks = true;
}