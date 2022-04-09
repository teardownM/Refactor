#include "lua.h"
#include "Teardown/Constructors.h"
#include "Teardown/Script.h"
#include "Teardown/CLuaFunctions.h"

void TDU::CLua::TestFunctions::TestFunc1(CScriptCore* pSC, lua_State*& L, CRetInfo* pRet) {
	pRet->retCount = 0;
	return;
}