#include "LuaDataModelClass.h"
#include "XmlDataModel.h"

XLLRTGlobalAPI LuaDataModelClass::mLuaDataModelClassMemberFunctions[] = 
{
	{"GetItemAtIndex",LuaDataModelClass::GetItemAtIndex},
	{"GetDataBatch",LuaDataModelClass::GetDataBatch},
	{"GetCount",LuaDataModelClass::GetCount},
	{"GetColumnCount",LuaDataModelClass::GetColumnCount},
	{"PrepareData",LuaDataModelClass::PrepareData},
	{"ReleaseData",LuaDataModelClass::ReleaseData},
	{"SetDataBatchReadyListener", LuaDataModelClass::SetDataBatchReadyListener},
	{"SetSingleDataReadyListener", LuaDataModelClass::SetSingleDataReadyListener},
    {"__gc",LuaDataModelClass::DeleteSelf},
    {NULL,NULL}
};

void LuaDataModelClass::RegisterClass(const char * DataModelClassName, XL_LRT_ENV_HANDLE hEnv)
{
    if(hEnv == NULL)
    {
        return;
    }

	long nLuaResult =  XLLRT_RegisterClass(hEnv,DataModelClassName,mLuaDataModelClassMemberFunctions,NULL,0);
}

DataModelInterface *LuaDataModelClass::GetDataModelClassObject(lua_State *luaState)
{
	//luaջ��, 1���������, ��2��ʼ�ǲ���
	const char* dataModelClassName = static_cast<const char*>(lua_tostring(luaState,2));
	DataModelInterface **ppDataModelInstance = reinterpret_cast<DataModelInterface**>(luaL_checkudata(luaState, 1, dataModelClassName));
	if(ppDataModelInstance && (*ppDataModelInstance))
		return *ppDataModelInstance;
	return 0;
}

int LuaDataModelClass::SetSingleDataReadyListener(lua_State* luaState)
{
	if (DataModelInterface *pDataModelClass = GetDataModelClassObject(luaState))
	{
		if(!lua_isfunction(luaState, 3)) //��ģ������2, ��ģ���ʱ��lua��û�м���������, �ýӿڵ�ʱ�����
		{
			return 0;
		}
		long functionRef = luaL_ref(luaState, LUA_REGISTRYINDEX);
		pDataModelClass->SetSingleDataReadyListener(reinterpret_cast<DWORD>(luaState), functionRef, LuaDataModelClass::LuaDataReadyListener);
	}
	return 0;
}

int LuaDataModelClass::SetDataBatchReadyListener(lua_State* luaState)
{
	if (DataModelInterface *pDataModelClass = GetDataModelClassObject(luaState))
	{
		if(!lua_isfunction(luaState, 3))//��ģ������2, ��ģ���ʱ��lua��û�м���������, �ýӿڵ�ʱ�����
		{
			return 0;
		}
		long functionRef = luaL_ref(luaState, LUA_REGISTRYINDEX);
		pDataModelClass->SetDataBatchReadyListener(reinterpret_cast<DWORD>(luaState), functionRef, LuaDataModelClass::LuaDataReadyListener);
	}
	return 0;
}

// lua is listening to "data ready" call from c++
void LuaDataModelClass::LuaDataReadyListener(DWORD dwUserData1, DWORD dwUserData2, int row, int column)
{
	lua_State *luaState = reinterpret_cast<lua_State*>(dwUserData1);
	int nNowTop = lua_gettop(luaState);
	lua_rawgeti(luaState, LUA_REGISTRYINDEX, dwUserData2);

	lua_pushinteger(luaState, row);
	lua_pushinteger(luaState, column);

	//����ԭ��XL_LRT_API(long) XLLRT_LuaCall(lua_State* luaState,int args,int results,const wchar_t* contextInfo);
	int nLuaResult = XLLRT_LuaCall(luaState, 2, 0, L"LuaDataModelClass::LuaDataReadyListener");
	lua_settop(luaState, nNowTop);
	return;
}

int LuaDataModelClass::PrepareData(lua_State *luaState)
{
	if (DataModelInterface *pDataModelClass = GetDataModelClassObject(luaState))
	{
		int from = static_cast<int>(lua_tointeger(luaState, 3));
		int to = static_cast<int>(lua_tointeger(luaState, 4));
		pDataModelClass->PrepareData(from, to);
		return 1;
	}
	return 1;
}

int LuaDataModelClass::ReleaseData(lua_State *luaState)
{
	if (DataModelInterface *pDataModelClass = GetDataModelClassObject(luaState))
	{
		int from = static_cast<int>(lua_tointeger(luaState, 3));
		int to = static_cast<int>(lua_tointeger(luaState, 4));
		pDataModelClass->ReleaseData(from, to);
		return 1;
	}
	return 1;
}

int LuaDataModelClass::GetDataBatch(lua_State *L)
{
	if(DataModelInterface *pDataModelClass = GetDataModelClassObject(L))
	{
		int from = static_cast<int>(lua_tointeger(L, 3));
		int to = static_cast<int>(lua_tointeger(L, 4));
		int columnCount = pDataModelClass->GetColumnCount();
		char** types = new char*[columnCount];
		void** dataBatch = new void*[columnCount*(to-from+1)];
		if (from <= 0) from = 1;
		if (to > pDataModelClass->GetCount()) to =  pDataModelClass->GetCount();

		if (pDataModelClass->GetDataBatch(from, to, dataBatch, types))
		{
			lua_newtable(L); // Ҫ���ص�table, �����ֽ�return table
			for ( int row = from; row <= to; row++)
			{
				lua_newtable(L);// ��һ�е�Table, ����rowtable, ����rowtable@-1
				void *itemData = dataBatch[3*(row-from)];
				for ( int column = 1; column<=columnCount; column++)
				{
					lua_pushnumber(L, column);//��indexѹջ, ����rowtable@-2
					void *itemData = dataBatch[3*(row-from)+column-1];
					if (strcmp(types[column-1], "char") == 0) //��һ�ΰ�ֵѹջ
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
						lua_pushnil(L); //��ʹû�ж�������, ҲҪ��һ����ռλ
					}
					//����value@-1, index@-2, rowtable@-3
					lua_settable(L, -3); // �Ѹ�ѹջ��index, value��������rowtable��
				}
				lua_pushnumber(L, row);//��һά����rowѹջ
				lua_insert(L, -2); // ��ջ����һά������rowtable��һ��λ��, 
				lua_settable(L, -3); // �Ѹ�ѹջ��һά����row, ��rowtable����, ����Ҫ���ص�return table��
			}
			return 1;
		}
	}
	return 1;
}


int LuaDataModelClass::GetItemAtIndex(lua_State *luaState)
{
	if (DataModelInterface *pDataModelClass = GetDataModelClassObject(luaState))
	{
		int row = static_cast<int> (lua_tointeger(luaState, 3));
		int column = static_cast<int>(lua_tointeger(luaState, 4));
		void *itemData;
		char* dataType = pDataModelClass->GetItemAtIndex(row, column, &itemData);
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


int LuaDataModelClass::GetCount(lua_State *luaState)
{
	if (DataModelInterface *pDataModelClass = GetDataModelClassObject(luaState))
	{
		int rowCount = pDataModelClass->GetCount();
		lua_pushinteger(luaState, rowCount);
		return 1;
	}
	lua_pushnil(luaState);
	return 1;
}


int LuaDataModelClass::GetColumnCount(lua_State *luaState)
{
	if (DataModelInterface *pDataModelClass = GetDataModelClassObject(luaState))
	{
		int columnCount = pDataModelClass->GetColumnCount();
		lua_pushinteger(luaState, columnCount);
		return 1;
	}
	lua_pushnil(luaState);
	return 1;
}


int LuaDataModelClass::DeleteSelf(lua_State *luaState)
{
	if(DataModelInterface *pDataModelClass = GetDataModelClassObject(luaState))
	{
		delete pDataModelClass;
	}
	return 0;
}

int LuaDataModelClassFactory::CreateInstance(lua_State* luaState)
{
	if (lua_isstring(luaState, 2) && lua_isstring(luaState, 3))
	{
		DataModelInterface* pResult = NULL;
		const char *dataModelClassName = static_cast<const char*>(lua_tostring(luaState, 2));
		const char *userdata = static_cast<const char*>(lua_tostring(luaState, 3));
		const char** argv = new const char*[1];
		argv[0] = userdata;
		if (strcmp(dataModelClassName, "XmlDataModel") == 0)
		{
			pResult = new XmlDataModel(1, argv); //ʵ��������
		}
		// �����������DataModelInterface��ʵ��, �������ﴦ��. 
		// �����µ�DataModelʱ, ������Ҫ�޸Ĺ������ʵ��, ���Ǳ�����һ��LuaDataModelClassʵ�ֵĴ����������ٵ�
		XLLRT_PushXLObject(luaState,dataModelClassName,pResult);
	}
    return 1;
}

LuaDataModelClassFactory* __stdcall LuaDataModelClassFactory::Instance(void*)
{
    static LuaDataModelClassFactory* s_pTheOne = NULL;
    if(s_pTheOne == NULL)
    {
        s_pTheOne = new LuaDataModelClassFactory();
    }
    return s_pTheOne;
}

XLLRTGlobalAPI LuaDataModelClassFactory::mLuaDataModelClassFactoryMemberFunctions[] = 
{
   {"CreateInstance",LuaDataModelClassFactory::CreateInstance},
    {NULL,NULL}
};

void LuaDataModelClassFactory::RegisterObj(const char *dataModelClassName, XL_LRT_ENV_HANDLE hEnv)
{
    if(hEnv == NULL)
	{
        return ;
	}

    XLLRTObject factoryObject;
	char* factoryClassName = new char[100];
	strcpy(factoryClassName, dataModelClassName);
	strcat(factoryClassName, ".Factory.Class");
	char* factoryObjectName = new char[100];
	strcpy(factoryObjectName, dataModelClassName);
	strcat(factoryObjectName, ".Factory.Object");
	factoryObject.ClassName =factoryClassName;
    factoryObject.MemberFunctions = mLuaDataModelClassFactoryMemberFunctions;
	factoryObject.ObjName =factoryObjectName;

    factoryObject.userData = NULL;
    factoryObject.pfnGetObject = (fnGetObject)LuaDataModelClassFactory::Instance;

    XLLRT_RegisterGlobalObj(hEnv,factoryObject); 
}