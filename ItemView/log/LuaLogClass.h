#pragma once

#include "./LogClass.h"

#define LOG_CLASS_LUA_CLASS "LogClass"
#define LOG_CLASS_FACTORY_LUA_CLASS "LogClass.Factory.Class"
#define LOG_CLASS_FACTORY_LUA_OBJ "LogClass.Factory"

/*依托lua本身与C/C++交互的机制，在lua中可以调用的C/C++方法，其原型须为static int function(lua_State)*/
/*按照这个原型实现你的业务逻辑中需要的方法时,从luastate的栈里面取出来参数,然后把要返回的结果压回到栈里面去.
*/

class LuaLogClass
{
public:
	static int log(lua_State * luaState);
	static int warn(lua_State * luaState);
	static int DeleteSelf(lua_State *luaState);
	static void RegisterClass(XL_LRT_ENV_HANDLE hEnv);
private:
	static LogClass *GetLogClass(lua_State *luaState);
};

//创建LogClass 实例的类厂单件
class LuaLogClassFactory
{
public:
	LogClass *CreateInstance();
	static LuaLogClassFactory * __stdcall Instance(void*);
	static int CreateInstance(lua_State *luaState);
	static void RegisterObj(XL_LRT_ENV_HANDLE hEnv);
};