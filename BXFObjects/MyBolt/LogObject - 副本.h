 #pragma once

#include "stdafx.h"

#define LOG_CLASS_LUA_CLASS "LogObject"
#define LOG_CLASS_LUA_FACTORY "LogObjectFactory"

class LogObject
{
public:
	static int DeleteSelf(lua_State *luaState);
	static void RegisterClass(XL_LRT_ENV_HANDLE hEnv);
	static int Log(lua_State *luaState);
private:
	static XLLRTGlobalAPI mLogObjectMemberFunctions[];
};

class LogObjectFactory
{
public:
	static LogObjectFactory * __stdcall Instance(void*);
	static int CreateInstance(lua_State *luaState);
	static void RegisterObj(XL_LRT_ENV_HANDLE hEnv);
private:
	static XLLRTGlobalAPI mLogObjectFactoryMemberFunctions[];
};