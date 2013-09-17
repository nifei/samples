#include "stdafx.h"
#include "LogObject.h"


XLLRTGlobalAPI LogObject::mLogObjectMemberFunctions[] = 
{
    {"__gc",LogObject::DeleteSelf},
	{"Log", LogObject::Log},
    {NULL,NULL}
};

XLLRTGlobalAPI LogObjectFactory::mLogObjectFactoryMemberFunctions[] = 
{
   {"CreateInstance",LogObjectFactory::CreateInstance},
    {NULL,NULL}
};

int LogObject::DeleteSelf(lua_State *luaState)
{
     LogObject** ppLogObject= reinterpret_cast<LogObject**>(luaL_checkudata(luaState,1,LOG_CLASS_LUA_CLASS));   
     if(ppLogObject)
     {
         delete (*ppLogObject);
     }
	 return 0;
}

int LogObject::Log(lua_State *L)
{
	LogObject **ppLogObject = reinterpret_cast<LogObject**>(luaL_checkudata(L, 1, "LogObject"));
	return 1;
}

void LogObject::RegisterClass(XL_LRT_ENV_HANDLE hEnv)
{
    if(hEnv == NULL)
    {
        return;
    }
	long nLuaResult =  XLLRT_RegisterClass(hEnv,LOG_CLASS_LUA_CLASS,mLogObjectMemberFunctions,NULL,0);
}

// create instance of LuaDir class
int LogObjectFactory::CreateInstance(lua_State* luaState)
{
	LogObject *obj = new LogObject(); //ʵ��������
	XLLRT_PushXLObject(luaState, "LogObject",obj);
	return 1;
}

// get instance of LuaDirFactory
LogObjectFactory* __stdcall LogObjectFactory::Instance(void*)
{
    static LogObjectFactory* s_pTheOne = NULL;
    if(s_pTheOne == NULL)
    {
        s_pTheOne = new LogObjectFactory();
    }
    return s_pTheOne;
}

void LogObjectFactory::RegisterObj(XL_LRT_ENV_HANDLE hEnv)
{
    if(hEnv == NULL)
	{
        return ;
	}

    XLLRTObject factoryObject;
	factoryObject.ClassName ="LogObjectFactory";
    factoryObject.MemberFunctions = mLogObjectFactoryMemberFunctions;
	factoryObject.ObjName =LOG_CLASS_LUA_FACTORY;

    factoryObject.userData = NULL;
    factoryObject.pfnGetObject = (fnGetObject)LogObjectFactory::Instance;

    XLLRT_RegisterGlobalObj(hEnv,factoryObject); 
}