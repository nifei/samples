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
	{"Click3", LuaDerived2::Click3},
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
	// 新接口: XLLRT_GetXLObjectClass (lua_State* luaState, int index);
	// lua_State当前栈底的对象时通过XLLRT_PushXLObject设置到Lua环境中的
	// Push的时候说它是什么(即使它不是), 这个className就是什么
	const char *className = XLLRT_GetXLObjectClass(L, 1);

	//这个类型转换会在ppDataObject其实是指向子类对象的时候崩溃
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
	// 新接口: long XLLRT_CheckXLObject(lua_State* luaState, int index, const char* className, void** lplpObj);
	void *lpObjBase, *lpObjDerived2;
	void *lpObjDerived1;

	// 该调用成功, 因为当前栈底作为"LUA_DERIVED2" push到lua环境中的对象是作为LUA_BASE的"子类"注册的
	long result = XLLRT_CheckXLObject(luaState, 1, LUA_BASE, &lpObjBase);

	// Check调用在"push一个Derived2对象, push的时候声称是LUA_DERIVED2"的时候崩坏
	// Get调用会返回错误并赋lpObjDerived1空值
	// 类名检查时检查是不是和push的时候声称的一样, 跟对象本身...
	long result1 = XLLRT_GetXLObject(luaState, 1, LUA_DERIVED1, &lpObjDerived1);

	// 该调用成功, 因为push的时候声称是LUA_DERIVED2
	// 即使push的时候传了一个实际指向LUA_DERIVED1的对象进去也成功
	long result2 = XLLRT_CheckXLObject(luaState, 1, LUA_DERIVED2, &lpObjDerived2);
	Derived2 **ppDataObject = reinterpret_cast<Derived2**>(lua_touserdata(luaState, 1));
	(*ppDataObject)->Click2();
	return 1;
}

int LuaDerived2::Click3(lua_State *luaState)
{
	// lua_touserdata 不会做类型检查, 返回void*
	// 如果使用者也不做类型检查就直接调用类方法的话, 存在访问不存在的非静态数据风险
	// XLLRT_GetXLObjectClass返回的是push的时候声称的类名, 如果push的时候就"名不副实"?
	Derived2 **ppDataObject = reinterpret_cast<Derived2**>(lua_touserdata(luaState, 1));
	(*ppDataObject)->Click3();
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
		// 如果在这里push的时候的类名不是子类而是基类的, 那么子类的放在在lua中无法调用. 
		// 
		XLLRT_PushXLObject(luaState, LUA_DERIVED1, obj);
	}
	else if (strcmp(className, "Derived2") == 0)
	{
		// 试试Base *obj = new Derived2();
		 Base *obj = new Derived1();
		// 即使在这里push一个Derived1的指针到lua环境中, 因为类名参数是LUA_DERIVED2, 所以回头XLLRT_GetXLObjectClass 取出的也是LUA_DERIVED2
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