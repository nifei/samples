#pragma once
#include <XLUE.h>

/*!
	LuaDataModelClassTemplate 是DataModel类的Lua封装类模板. 
	LuaDataModelClassFactoryTemplate 是创建LuaDataModelClassTemplate<DataModel>实例的工厂类模板

	如果用户提供的DataModel类提供以下函数, 就可以直接使用这个模板:

	GetItemAtIndex(const char* userdata, int row, int column)
	GetCount(const char* userdata)
	GetColumnCount(const char *userdata)

	如果ItemView的PreloadDataCount属性为0, 即不想要提前加载数据, 以下方法不会被调用. 
	PrepareData(const char *userdata)
	ReleaseData(const char *userdata)
	AttachDataReadyListener(const char* userdata)
	但是如果你一定要用现成的模板类而不愿意自己实现一个DataMode的Lua封装类的话, 就必须提供上面的三个方法保证编译通过. 
*/
template <class TDataModelClass>
class LuaDataModelClassTemplate
{
public:
	static int DeleteSelf(lua_State *luaState);
	static int GetItemAtIndex(lua_State *luaState);
	static int GetDataBatch(lua_State *luaState);
	static int GetCount(lua_State *luaState);
	static int GetColumnCount(lua_State *luaState);
	static int PrepareData(lua_State *luaState);
	static int ReleaseData(lua_State *luaState);
	/*!
		这个方法和LuaDataModelClassTemplate<DataModel>::SetDataBatchReadyListener的用法是一样的. 不同之处在于SetDataBatchReadyListener的回调方法DataBatchReady会在一批数据准备好的时候被调用. 例如ItemView调用PrepareData(1, 100), 那么在这个100个数据准备好以后, DataModel应该调用DataBatchReady来通知ItemView有一批数据已经可以被调用了. 而SetSingleDataReadyListener传入的回调方法应该在每一个单元格的数据就绪之后都被调用一次, 回调传的参数是就绪单元格的行与列
	*/
	static int SetSingleDataReadyListener(lua_State *luaState);
	/*!
		如果用户提供给ItemView的DataModelCallbackTable中包含SetDataBatchReadyListener, 那么ItemView会调用该方法传给LuaDataModelClassTemplate<DataModel>一个Lua回调方法DataReadyCallback在数据准备好的时候通知ItemView
		LuaDataModelClassTemplate<DataModel>会把DataReadyCallback连同回调时需要的参数dwUserData1, dwUserData2原封不动地传给DataModel, 当DataModel准备好数据的时候应该调用DataReadyCallback. 但是DataModel不应该直接操作lua接口, 所以除了lua方法DataReadyCallback和dwUserData1, dwUserData2之外, LuaDataModelClassTemplate<DataModel>会把自己的方法LuaDataModelClassTemplate<DataModel>::LuaDataReadyListener也作为参数传给DataModel, DataModel回调这个方法通知数据准备好了就可以了. 
		///sa AttachDataReadyListener
	*/
	static int SetDataBatchReadyListener(lua_State *luaState);
	static void RegisterClass(XL_LRT_ENV_HANDLE hEnv);
	static void LuaDataReadyListener(DWORD dwUserData1, DWORD dwUserData2, int row, int column);
private:
	static TDataModelClass *GetDataModelClass(lua_State *luaState);
	static XLLRTGlobalAPI mLuaDataModelClassMemberFunctions[];
};

template <class TDataModelClass>
class LuaDataModelClassFactoryTemplate
{
public:
	static LuaDataModelClassFactoryTemplate * __stdcall Instance(void*);
	static int CreateInstance(lua_State *luaState);
	static void RegisterObj(XL_LRT_ENV_HANDLE hEnv);
private:
	static XLLRTGlobalAPI mLuaDataModelClassFactoryMemberFunctions[];
};

template <class TDataModelClass>
XLLRTGlobalAPI LuaDataModelClassTemplate<TDataModelClass>::mLuaDataModelClassMemberFunctions[] = 
{
	{"GetItemAtIndex",LuaDataModelClassTemplate<TDataModelClass>::GetItemAtIndex},
	{"GetDataBatch",LuaDataModelClassTemplate<TDataModelClass>::GetDataBatch},
	{"GetCount",LuaDataModelClassTemplate<TDataModelClass>::GetCount},
	{"GetColumnCount",LuaDataModelClassTemplate<TDataModelClass>::GetColumnCount},
	{"PrepareData",LuaDataModelClassTemplate<TDataModelClass>::PrepareData},
	{"ReleaseData",LuaDataModelClassTemplate<TDataModelClass>::ReleaseData},
	{"SetDataBatchReadyListener", LuaDataModelClassTemplate<TDataModelClass>::SetDataBatchReadyListener},
	{"SetSingleDataReadyListener", LuaDataModelClassTemplate<TDataModelClass>::SetSingleDataReadyListener},
    {"__gc",LuaDataModelClassTemplate<TDataModelClass>::DeleteSelf},
    {NULL,NULL}
};

template <class TDataModelClass>
void LuaDataModelClassTemplate<TDataModelClass>::RegisterClass(XL_LRT_ENV_HANDLE hEnv)
{
    if(hEnv == NULL)
    {
        return;
    }

	long nLuaResult =  XLLRT_RegisterClass(hEnv,TDataModelClass::GetDataModelClassName(),mLuaDataModelClassMemberFunctions,NULL,0);
}

template <class TDataModelClass>
TDataModelClass *LuaDataModelClassTemplate<TDataModelClass>::GetDataModelClass(lua_State *luaState)
{
	TDataModelClass **ppDataModelClass = reinterpret_cast<TDataModelClass**>(luaL_checkudata(luaState, 1, TDataModelClass::GetDataModelClassName()));
	if(ppDataModelClass && (*ppDataModelClass))
		return *ppDataModelClass;
	return 0;
}

template<class TDataModelClass>
int LuaDataModelClassTemplate<TDataModelClass>::SetSingleDataReadyListener(lua_State* luaState)
{
	if (TDataModelClass *pDataModelClass = GetDataModelClass(luaState))
	{
		if(!lua_isfunction(luaState, 2))
		{
			return 0;
		}
		long functionRef = luaL_ref(luaState, LUA_REGISTRYINDEX);
		pDataModelClass->SetSingleDataReadyListener(reinterpret_cast<DWORD>(luaState), functionRef, LuaDataModelClassTemplate<TDataModelClass>::LuaDataReadyListener);
	}
	return 0;
}

template<class TDataModelClass>
int LuaDataModelClassTemplate<TDataModelClass>::SetDataBatchReadyListener(lua_State* luaState)
{
	if (TDataModelClass *pDataModelClass = GetDataModelClass(luaState))
	{
		if(!lua_isfunction(luaState, 2))
		{
			return 0;
		}
		long functionRef = luaL_ref(luaState, LUA_REGISTRYINDEX);
		pDataModelClass->SetDataBatchReadyListener(reinterpret_cast<DWORD>(luaState), functionRef, LuaDataModelClassTemplate<TDataModelClass>::LuaDataReadyListener);
	}
	return 0;
}

template<class TDataModelClass>
// lua is listening to "data ready" call from c++
void LuaDataModelClassTemplate<TDataModelClass>::LuaDataReadyListener(DWORD dwUserData1, DWORD dwUserData2, int row, int column)
{
	lua_State *luaState = reinterpret_cast<lua_State*>(dwUserData1);
	int nNowTop = lua_gettop(luaState);
	lua_rawgeti(luaState, LUA_REGISTRYINDEX, dwUserData2);

	lua_pushinteger(luaState, row);
	lua_pushinteger(luaState, column);

	int nLuaResult = XLLRT_LuaCall(luaState, 2, 0, L"LuaDataModelClass::LuaDataReadyListener");
	lua_settop(luaState, nNowTop);
	return;
}

template<class TDataModelClass>
int LuaDataModelClassTemplate<TDataModelClass>::PrepareData(lua_State *luaState)
{
	if (TDataModelClass *pDataModelClass = GetDataModelClass(luaState))
	{
		int from = static_cast<int>(lua_tointeger(luaState, 3));
		int to = static_cast<int>(lua_tointeger(luaState, 4));
		const char* userdata = static_cast<const char*>(lua_tostring(luaState,2));
		pDataModelClass->PrepareData(from, to, userdata);
		return 1;
	}
	return 1;
}

template<class TDataModelClass>
int LuaDataModelClassTemplate<TDataModelClass>::ReleaseData(lua_State *luaState)
{
	if (TDataModelClass *pDataModelClass = GetDataModelClass(luaState))
	{
		int from = static_cast<int>(lua_tointeger(luaState, 3));
		int to = static_cast<int>(lua_tointeger(luaState, 4));
		const char* userdata = static_cast<const char*>(lua_tostring(luaState,2));
		pDataModelClass->ReleaseData(from, to, userdata);
		return 1;
	}
	return 1;
}

template<class TDataModelClass>
int LuaDataModelClassTemplate<TDataModelClass>::GetDataBatch(lua_State *L)
{
	if(TDataModelClass *pDataModelClass = GetDataModelClass(L))
	{
		const char* view = static_cast<const char*>(lua_tostring(L, 2));
		int from = static_cast<int>(lua_tointeger(L, 3));
		int to = static_cast<int>(lua_tointeger(L, 4));
		int columnCount = pDataModelClass->GetColumnCount(view);
		char** types = new char*[columnCount];
		void** dataBatch = new void*[columnCount*(to-from+1)];
		if (from <= 0) from = 1;
		if (to > pDataModelClass->GetCount(view)) to =  pDataModelClass->GetCount(view);

		if (pDataModelClass->GetDataBatch(from, to, view, dataBatch, types))
		{
			lua_newtable(L); // 要返回的table, 起名字叫return table
			for ( int row = from; row <= to; row++)
			{
				lua_newtable(L);// 这一行的Table, 叫它rowtable, 现在rowtable@-1
				void *itemData = dataBatch[3*(row-from)];
				for ( int column = 1; column<=columnCount; column++)
				{
					lua_pushnumber(L, column);//把index压栈, 现在rowtable@-2
					void *itemData = dataBatch[3*(row-from)+column-1];
					if (strcmp(types[column-1], "char") == 0) //这一段把值压栈
					{
						char* cData = (char*)itemData;
						lua_pushstring(L, cData);
					} else if (strcmp(types[column-1], "bitmap") == 0)
					{
						XL_BITMAP_HANDLE *hBitmap = (XL_BITMAP_HANDLE*)itemData;	
						if (*hBitmap)
							XLUE_PushBitmap(L, *hBitmap);
						else
							lua_pushnil(L);
					} else
					{
						lua_pushnil(L); //即使没有读到数据, 也要放一个空占位
					}
					//现在value@-1, index@-2, rowtable@-3
					lua_settable(L, -3); // 把刚压栈的index, value弹出放在rowtable中
				}
				lua_pushnumber(L, row);//把一维索引row压栈
				lua_insert(L, -2); // 把栈顶的一维索引和rowtable换一下位置, 
				lua_settable(L, -3); // 把刚压栈的一维索引row, 和rowtable弹出, 放在要返回的return table中
			}
			return 1;
		}
	}
	return 1;
}

template <class TDataModelClass>
int LuaDataModelClassTemplate<TDataModelClass>::GetItemAtIndex(lua_State *luaState)
{
	if (TDataModelClass *pDataModelClass = GetDataModelClass(luaState))
	{
		int row = static_cast<int> (lua_tointeger(luaState, 3));
		int column = static_cast<int>(lua_tointeger(luaState, 4));
		const char* view = static_cast<const char*> (lua_tostring(luaState, 2));
		void *itemData;
		char* dataType = pDataModelClass->GetItemAtIndex(row, column, view, &itemData);
		if (strcmp(dataType, "char") == 0)
		{
			char* cData = (char*)itemData;
			lua_pushstring(luaState, cData);
		} else if (strcmp(dataType, "bitmap") == 0)
		{
			XL_BITMAP_HANDLE hBitmap = itemData;	
			XLUE_PushBitmap(luaState, hBitmap);
		}
		else {
			lua_pushnil(luaState);
		}
		return 1;
	}
	lua_pushnil(luaState);
	return 1;
}

template <class TDataModelClass>
int LuaDataModelClassTemplate<TDataModelClass>::GetCount(lua_State *luaState)
{
	if (TDataModelClass *pDataModelClass = GetDataModelClass(luaState))
	{
		const char *view = static_cast<const char*>(lua_tostring(luaState, 2));
		int rowCount = pDataModelClass->GetCount(view);
		lua_pushinteger(luaState, rowCount);
		return 1;
	}
	lua_pushnil(luaState);
	return 1;
}

template <class TDataModelClass>
int LuaDataModelClassTemplate<TDataModelClass>::GetColumnCount(lua_State *luaState)
{
	if (TDataModelClass *pDataModelClass = GetDataModelClass(luaState))
	{
		const char *view = static_cast<const char*>(lua_tostring(luaState, 2));
		int columnCount = pDataModelClass->GetColumnCount(view);
		lua_pushinteger(luaState, columnCount);
		return 1;
	}
	lua_pushnil(luaState);
	return 1;
}

template <class TDataModelClass>
int LuaDataModelClassTemplate<TDataModelClass>::DeleteSelf(lua_State *luaState)
{
	if(TDataModelClass *pDataModelClass = GetDataModelClass(luaState))
	{
		delete pDataModelClass;
	}
	return 0;
}

template <class TDataModelClass>
int LuaDataModelClassFactoryTemplate<TDataModelClass>::CreateInstance(lua_State* luaState)
{
	TDataModelClass* pResult = NULL;
	if (lua_isstring(luaState, 2))
	{
		const char *userdata = static_cast<const char*>(lua_tostring(luaState, 2));
		const char** argv = new const char*[1];
		argv[0] = userdata;
		pResult = new TDataModelClass(1, argv);
	} else if (lua_istable(luaState, 2))
	{
		size_t size =  lua_objlen (luaState, 2);
		const char ** argv = new const char*[size];
		for ( size_t i = 1; i <=size; i++)
		{
			lua_pushinteger(luaState, i);
			lua_gettable(luaState, 2);
			const char* test = lua_tostring(luaState, -1);
			argv[i-1] = test;
		}
		pResult = new TDataModelClass(size, argv);
	} else {
		pResult = new TDataModelClass(0, 0);
	}
	XLLRT_PushXLObject(luaState,TDataModelClass::GetDataModelClassName(),pResult);
    return 1;
}

template <class TDataModelClass>
LuaDataModelClassFactoryTemplate<TDataModelClass>* __stdcall LuaDataModelClassFactoryTemplate<TDataModelClass>::Instance(void*)
{
    static LuaDataModelClassFactoryTemplate<TDataModelClass>* s_pTheOne = NULL;
    if(s_pTheOne == NULL)
    {
        s_pTheOne = new LuaDataModelClassFactoryTemplate<TDataModelClass>();
    }
    return s_pTheOne;
}

template <class TDataModelClass>
XLLRTGlobalAPI LuaDataModelClassFactoryTemplate<TDataModelClass>::mLuaDataModelClassFactoryMemberFunctions[] = 
{
   {"CreateInstance",LuaDataModelClassFactoryTemplate<TDataModelClass>::CreateInstance},
    {NULL,NULL}
};

template <class TDataModelClass>
void LuaDataModelClassFactoryTemplate<TDataModelClass>::RegisterObj(XL_LRT_ENV_HANDLE hEnv)
{
    if(hEnv == NULL)
	{
        return ;
	}

    XLLRTObject factoryObject;
	factoryObject.ClassName =TDataModelClass::GetDataModelFactoryClassName();
    factoryObject.MemberFunctions = mLuaDataModelClassFactoryMemberFunctions;
	factoryObject.ObjName =TDataModelClass::GetDataModelFactoryObjectName();

    factoryObject.userData = NULL;
    factoryObject.pfnGetObject = (fnGetObject)LuaDataModelClassFactoryTemplate::Instance;

    XLLRT_RegisterGlobalObj(hEnv,factoryObject); 
}