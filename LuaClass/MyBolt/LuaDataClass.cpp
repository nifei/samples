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
	// �½ӿ�: XLLRT_GetXLObjectClass (lua_State* luaState, int index);
	// lua_State��ǰջ�׵Ķ���ʱͨ��XLLRT_PushXLObject���õ�Lua�����е�
	// Push��ʱ��˵����ʲô(��ʹ������), ���className����ʲô
	const char *className = XLLRT_GetXLObjectClass(L, 1);

	//�������ת������ppDataObject��ʵ��ָ����������ʱ�����
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
	// �½ӿ�: long XLLRT_CheckXLObject(lua_State* luaState, int index, const char* className, void** lplpObj);
	void *lpObjBase, *lpObjDerived2;
	void *lpObjDerived1;

	// �õ��óɹ�, ��Ϊ��ǰջ����Ϊ"LUA_DERIVED2" push��lua�����еĶ�������ΪLUA_BASE��"����"ע���
	long result = XLLRT_CheckXLObject(luaState, 1, LUA_BASE, &lpObjBase);

	// Check������"pushһ��Derived2����, push��ʱ��������LUA_DERIVED2"��ʱ�����
	// Get���û᷵�ش��󲢸�lpObjDerived1��ֵ
	// �������ʱ����ǲ��Ǻ�push��ʱ�����Ƶ�һ��, ��������...
	long result1 = XLLRT_GetXLObject(luaState, 1, LUA_DERIVED1, &lpObjDerived1);

	// �õ��óɹ�, ��Ϊpush��ʱ��������LUA_DERIVED2
	// ��ʹpush��ʱ����һ��ʵ��ָ��LUA_DERIVED1�Ķ����ȥҲ�ɹ�
	long result2 = XLLRT_CheckXLObject(luaState, 1, LUA_DERIVED2, &lpObjDerived2);
	Derived2 **ppDataObject = reinterpret_cast<Derived2**>(lua_touserdata(luaState, 1));
	(*ppDataObject)->Click2();
	return 1;
}

int LuaDerived2::Click3(lua_State *luaState)
{
	// lua_touserdata ���������ͼ��, ����void*
	// ���ʹ����Ҳ�������ͼ���ֱ�ӵ����෽���Ļ�, ���ڷ��ʲ����ڵķǾ�̬���ݷ���
	// XLLRT_GetXLObjectClass���ص���push��ʱ�����Ƶ�����, ���push��ʱ���"������ʵ"?
	Derived2 **ppDataObject = reinterpret_cast<Derived2**>(lua_touserdata(luaState, 1));
	(*ppDataObject)->Click3();
	return 1;
}

int LuaFactory::CreateInstance(lua_State* luaState)
{
	const char *className = lua_tostring(luaState, 2);
	if (strcmp(className, "Base") == 0)
	{
		Base *obj = new Base(); //ʵ��������
		XLLRT_PushXLObject(luaState, LUA_BASE, obj);
	}
	else if (strcmp(className, "Derived1") == 0)
	{
		Base *obj = new Derived1();
		// ���������push��ʱ�����������������ǻ����, ��ô����ķ�����lua���޷�����. 
		// 
		XLLRT_PushXLObject(luaState, LUA_DERIVED1, obj);
	}
	else if (strcmp(className, "Derived2") == 0)
	{
		// ����Base *obj = new Derived2();
		 Base *obj = new Derived1();
		// ��ʹ������pushһ��Derived1��ָ�뵽lua������, ��Ϊ����������LUA_DERIVED2, ���Ի�ͷXLLRT_GetXLObjectClass ȡ����Ҳ��LUA_DERIVED2
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