#include "stdafx.h"

#include "LuaDataClass.h"
#include "DataClass.h"

XLLRTGlobalAPI LuaBase::s_LuaBaseMemberFunctions[] = 
{
	{"Click", LuaBase::Click},
    {"__gc", LuaBase::DeleteSelf},
    {NULL,NULL}
};

XLLRTGlobalAPI LuaDerived1::s_LuaDerived1MemberFunctions[] = 
{
	{"Click1", LuaDerived1::Click1},
    {NULL,NULL}
};

XLLRTGlobalAPI LuaDerived2::s_LuaDerived2MemberFunctions[] = 
{
	{"Click2", LuaDerived2::Click2},
    {NULL,NULL}
};

XLLRTGlobalAPI LuaFactory::s_LuaFactoryMemeberFunctions[] = 
{
   {"CreateInstance",LuaFactory::CreateInstance},
    {NULL,NULL}
};

int LuaBase::DeleteSelf(lua_State *luaState)
{
	//lua_touserdata
     Base** ppDataObject= reinterpret_cast<Base**>(luaL_checkudata(luaState, 1, LUA_BASE));   
     if(ppDataObject)
     {
         delete (*ppDataObject);
     }
	 return 0;
}

int LuaBase::Click(lua_State *L)
{
	//lua_touserdata
	//如果是子类push到lua环境中的话这个类型转换会崩溃. 
	//Base **ppDataObject = reinterpret_cast<Base**>(luaL_checkudata(L, 1, LUA_BASE));
	Base **ppDataObject = reinterpret_cast<Base**>(lua_touserdata(L, 1));
	(*ppDataObject)->Click();
	return 1;
}

void LuaBase::RegisterClass(XL_LRT_ENV_HANDLE hEnv)
{
    if(hEnv == NULL)
    {
        return;
    }
	long nLuaResult =  XLLRT_RegisterClass(hEnv, LUA_BASE, s_LuaBaseMemberFunctions,NULL,0);
}

void LuaDerived1::RegisterClass(XL_LRT_ENV_HANDLE hEnv)
{
    if(hEnv == NULL)
    {
        return;
    }
	long nLuaResult =  XLLRT_RegisterClass(hEnv, LUA_DERIVED1, s_LuaDerived1MemberFunctions,LUA_BASE,0);
}

int LuaDerived1::Click1(lua_State *luaState)
{
	Derived1 **ppDataObject = reinterpret_cast<Derived1**>(lua_touserdata(luaState, 1));
	(*ppDataObject)->Click1();
	return 1;
}

void LuaDerived2::RegisterClass(XL_LRT_ENV_HANDLE hEnv)
{
    if(hEnv == NULL)
    {
        return;
    }
	long nLuaResult =  XLLRT_RegisterClass(hEnv, LUA_DERIVED2, s_LuaDerived2MemberFunctions,LUA_BASE,0);
}

int LuaDerived2::Click2(lua_State *luaState)
{
	Derived2 **ppDataObject = reinterpret_cast<Derived2**>(lua_touserdata(luaState, 1));
	(*ppDataObject)->Click2();
	return 1;
}

int LuaFactory::CreateInstance(lua_State* luaState)
{
	const char *className = lua_tostring(luaState, 2);
	if (strcmp(className, "Base") == 0)
	{
		Base *obj = new Base(); //实例化问题
		XLLRT_PushXLObject(luaState, LUA_BASE, obj);
	}
	else if (strcmp(className, "Derived1") == 0)
	{
		Base *obj = new Derived1();
		XLLRT_PushXLObject(luaState, LUA_DERIVED1, obj);
	}
	else if (strcmp(className, "Derived2") == 0)
	{
		Base *obj = new Derived2();
		XLLRT_PushXLObject(luaState, LUA_DERIVED2, obj);
	}
	return 1;
}

// get instance of LuaDirFactory
LuaFactory* __stdcall LuaFactory::Instance(void*)
{
    static LuaFactory* s_pTheOne = NULL;
    if(s_pTheOne == NULL)
    {
        s_pTheOne = new LuaFactory();
    }
    return s_pTheOne;
}

void LuaFactory::RegisterObj(XL_LRT_ENV_HANDLE hEnv)
{
    if(hEnv == NULL)
	{
        return ;
	}

    XLLRTObject factoryObject;
	factoryObject.ClassName ="LuaFactory";
    factoryObject.MemberFunctions = s_LuaFactoryMemeberFunctions;
	factoryObject.ObjName =LUA_BASE_FACTORY;

    factoryObject.userData = NULL;
    factoryObject.pfnGetObject = (fnGetObject)LuaFactory::Instance;

    XLLRT_RegisterGlobalObj(hEnv,factoryObject); 
}