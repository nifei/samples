#ifndef _LUA_DATA_CLASS_H_
#define _LUA_DATA_CLASS_H_
#include "stdafx.h"
#pragma once

#define LUA_BASE "LuaBase"
#define LUA_DERIVED1 "LuaDerived1"
#define LUA_DERIVED2 "LuaDerived2"
#define LUA_BASE_FACTORY "LuaBaseFactoryObject"

class LuaBase
{
public:
	static int DeleteSelf(lua_State *luaState);
	static void RegisterClass(XL_LRT_ENV_HANDLE hEnv);
	static int Click(lua_State *luaState);
private:
	static XLLRTGlobalAPI s_LuaBaseMemberFunctions[];
};

class LuaDerived1
{
public:
	static int Click1(lua_State *luaState);
	static void RegisterClass(XL_LRT_ENV_HANDLE hEnv);
private:
	static XLLRTGlobalAPI s_LuaDerived1MemberFunctions[];
};

class LuaDerived2
{
public:
	static int Click2(lua_State *luaState);
	static void RegisterClass(XL_LRT_ENV_HANDLE hEnv);
private:
	static XLLRTGlobalAPI s_LuaDerived2MemberFunctions[];
};

class LuaFactory
{
public:
	static LuaFactory * __stdcall Instance(void*);
	static int CreateInstance(lua_State *luaState);
	static void RegisterObj(XL_LRT_ENV_HANDLE hEnv);
private:
	static XLLRTGlobalAPI s_LuaFactoryMemeberFunctions[];
};
#endif