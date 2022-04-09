#pragma once
#include "Teardown/Constructors.h"
#include "Teardown/Script.h"

class CRetInfo
{
public:
	lua_State* L;
	int retCount;
	int maxRet;
};

namespace TDU
{
	namespace CLua
	{

		namespace TestFunctions
		{
			void TestFunc1(CScriptCore* pSC, lua_State*& L, CRetInfo* ret);
		}

		inline void RegisterCFunctions(CScriptCore_LuaState* pSCLS)
		{
			Teardown::Constructors::RegisterLuaFunction(pSCLS, "http_put", TestFunctions::TestFunc1);
		}
	}
}