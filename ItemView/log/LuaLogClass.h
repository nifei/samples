#pragma once

#include "./LogClass.h"

#define LOG_CLASS_LUA_CLASS "LogClass"
#define LOG_CLASS_FACTORY_LUA_CLASS "LogClass.Factory.Class"
#define LOG_CLASS_FACTORY_LUA_OBJ "LogClass.Factory"

/*����lua������C/C++�����Ļ��ƣ���lua�п��Ե��õ�C/C++��������ԭ����Ϊstatic int function(lua_State)*/
/*�������ԭ��ʵ�����ҵ���߼�����Ҫ�ķ���ʱ,��luastate��ջ����ȡ��������,Ȼ���Ҫ���صĽ��ѹ�ص�ջ����ȥ.
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

//����LogClass ʵ�����೧����
class LuaLogClassFactory
{
public:
	LogClass *CreateInstance();
	static LuaLogClassFactory * __stdcall Instance(void*);
	static int CreateInstance(lua_State *luaState);
	static void RegisterObj(XL_LRT_ENV_HANDLE hEnv);
};