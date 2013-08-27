#include "stdafx.h"
#include "LuaLogClass.h"

LogClass *LuaLogClass::GetLogClass(lua_State *luaState)
{
	//仅仅是重新解释了给出的对象的比特模型而没有进行二进制转换。
	LogClass **ppLogClass = reinterpret_cast<LogClass**>(luaL_checkudata(luaState, 1, LOG_CLASS_LUA_CLASS));
	if(ppLogClass && (*ppLogClass))
		return *ppLogClass;
	return 0;
}

int LuaLogClass::log(lua_State *luaState)
{
	if (LogClass *pLogClass = GetLogClass(luaState))
	{
		const char* lhs = static_cast<const char*> (lua_tostring(luaState, 2));
		pLogClass->Log(lhs);
		lua_pushinteger(luaState, 1);
		return 1;
	}

	lua_pushnil(luaState);
	return 1;
}


int LuaLogClass::warn(lua_State *luaState)
{
	if (LogClass *pLogClass = GetLogClass(luaState))
	{
		const char* lhs = static_cast<const char*> (lua_tostring(luaState, 2));
		pLogClass->Warn(lhs);
		lua_pushinteger(luaState, 1);
		return 1;
	}

	lua_pushnil(luaState);
	return 1;
}


int LuaLogClass::DeleteSelf(lua_State *luaState)
{
	if(LogClass *pLogClass = GetLogClass(luaState))
	{
		delete pLogClass;
	}
	return 0;
}

static XLLRTGlobalAPI LuaLogClassMemberFunctions[] = 
{
	{"Log",LuaLogClass::log},
	{"Warn",LuaLogClass::warn},
    {"__gc",LuaLogClass::DeleteSelf},
    {NULL,NULL}
};

void LuaLogClass::RegisterClass(XL_LRT_ENV_HANDLE hEnv)
{
    if(hEnv == NULL)
    {
        return;
    }

    long nLuaResult = XLLRT_RegisterClass(hEnv,LOG_CLASS_LUA_CLASS,LuaLogClassMemberFunctions,NULL,0);
}
//------------------------------------------------------------------
LogClass* LuaLogClassFactory::CreateInstance()
{
    return new LogClass();
}

int LuaLogClassFactory::CreateInstance(lua_State* luaState)
{
    LogClass* pResult = new LogClass();
    XLLRT_PushXLObject(luaState,LOG_CLASS_LUA_CLASS,pResult);
    return 1;
}

LuaLogClassFactory* __stdcall LuaLogClassFactory::Instance(void*)
{
    static LuaLogClassFactory* s_pTheOne = NULL;
    if(s_pTheOne == NULL)
    {
        s_pTheOne = new LuaLogClassFactory();
    }
    return s_pTheOne;
}

static XLLRTGlobalAPI LuaLogClassFactoryMemberFunctions[] = 
{
    {"CreateInstance",LuaLogClassFactory::CreateInstance},
    {NULL,NULL}
};

void LuaLogClassFactory::RegisterObj(XL_LRT_ENV_HANDLE hEnv)
{
    if(hEnv == NULL)
	{
        return ;
	}

    XLLRTObject theObject;
    theObject.ClassName = LOG_CLASS_FACTORY_LUA_CLASS;
    theObject.MemberFunctions = LuaLogClassFactoryMemberFunctions;
    theObject.ObjName = LOG_CLASS_FACTORY_LUA_OBJ;
    theObject.userData = NULL;
    theObject.pfnGetObject = (fnGetObject)LuaLogClassFactory::Instance;

    XLLRT_RegisterGlobalObj(hEnv,theObject); 
}