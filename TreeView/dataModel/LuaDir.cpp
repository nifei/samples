#include "LuaDir.h"
#include ".\xl_lib\text\transcode.h"

void ListDirectoryContents(lua_State *L)
{
	const char* aDir = lua_tostring(L, 2);
	std::wstring wtemp;
	xl::text::transcode::UTF8_to_Unicode(aDir, MAX_PATH, wtemp);
	const wchar_t *wDir =wtemp.c_str();

	if (wcscmp(wDir, L"") == 0)
	{
		wchar_t szLogicalDriveStrings[MAX_PATH];                          //��ȡ���������ڴ�
		wchar_t *szDrive;
		::ZeroMemory(szLogicalDriveStrings,MAX_PATH);                    //���ڴ����㣬��һ�������������ַ��ĵ�ַ
		::GetLogicalDriveStrings(MAX_PATH-1,szLogicalDriveStrings);      //��ȡ�����е�����������
		szDrive=(wchar_t*)szLogicalDriveStrings;

		lua_newtable(L);
		int index = 1;

		do
		{
			int len = wcslen(szDrive);
			std::string atemp;
			xl::text::transcode::Unicode_to_UTF8(szDrive, MAX_PATH, atemp);
			const char* volume = atemp.c_str();
			UINT uDriveType = GetDriveType(szDrive);

			lua_newtable(L);
			lua_pushnumber(L, 1);
			lua_pushboolean(L, true); // has children
			lua_settable(L, -3);

			lua_pushnumber(L, 2);
			lua_pushstring(L, volume);
			lua_settable(L, -3);

			lua_pushnumber(L, index);
			lua_insert(L, -2); // ��ջ����һά������rowtable��һ��λ��, 
			lua_settable(L, -3); // �Ѹ�ѹջ��һά����row, ��rowtable����, ����Ҫ���ص�return table��
			index++;

			szDrive += (len+1);
		}
		while(*szDrive!='\x00');
	}
	else 
	{
		WIN32_FIND_DATA fdFile;
		HANDLE hFind = NULL;
		wchar_t wPath[MAX_PATH];
		wsprintf(wPath, L"%s\\*", wDir);

		if( (hFind = FindFirstFile(wPath, &fdFile)) == INVALID_HANDLE_VALUE)
		{
			return;
		}
		int index = 1;
		lua_newtable(L);
		do{
			if (wcscmp(fdFile.cFileName, L".") != 0 && wcscmp(fdFile.cFileName, L"..") != 0 
				&& !(fdFile.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)
				&& !(fdFile.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))
			{
				lua_newtable(L);
				lua_pushnumber(L, 1);
				if (fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{			
					lua_pushboolean(L, true);
				} else {
					lua_pushboolean(L, false);
				}
				lua_settable(L, -3);

				std::string subDirString;
				xl::text::transcode::Unicode_to_UTF8(fdFile.cFileName, MAX_PATH, subDirString);

				lua_pushnumber(L, 2);
				lua_pushstring(L, subDirString.c_str());
				lua_settable(L, -3);
				
				lua_pushnumber(L, index);//��һά����rowѹջ
				lua_insert(L, -2); // ��ջ����һά������rowtable��һ��λ��, 
				lua_settable(L, -3); // �Ѹ�ѹջ��һά����row, ��rowtable����, ����Ҫ���ص�return table��
				index++;
			}
		} while(::FindNextFile(hFind, &fdFile));
		::FindClose(hFind);
	}
	return;
}

/*��lua�����У���__��ͷ�ķ���һ��Ϊ�ڽ��������Ա��__gc������lua ���������Ͷ������Դ���շ�������Ϊlua�����ǻ����������յģ���һ���������Ͷ�����ʹ��ʱ���������ճ���Ὣ�ö����lua�������ͷţ����ҵ������ϵ�__gc�������ͷŸö���ռ�õ���Դ���μ�lua���飩������lua�����д���LuaMyClass��ʵ��ʱ���ڶ��ϴ�����һ��MyClass��ʵ�����ԳƵģ���LuaMyClassʵ�����һ����б�����ʱ����Ҫ��__gc�����дӶ����ͷŶ�Ӧ��MyClass��ʵ����
*/
XLLRTGlobalAPI LuaDir::mLuaDirMemberFunctions[] = 
{
	{"GetSubDirs", LuaDir::GetSubDirs},
    {"__gc",LuaDir::DeleteSelf},
    {NULL,NULL}
};

XLLRTGlobalAPI LuaDirFactory::mLuaDirFactoryMemberFunctions[] = 
{
   {"CreateInstance",LuaDirFactory::CreateInstance},
    {NULL,NULL}
};

int LuaDir::DeleteSelf(lua_State *luaState)
{
	return 0;
}

int LuaDir::GetSubDirs(lua_State *L)
{
	LuaDir **ppLuaDirInstance = reinterpret_cast<LuaDir**>(luaL_checkudata(L, 1, "LuaDir"));
	ListDirectoryContents(L);
	return 1;
}

void LuaDir::RegisterClass(XL_LRT_ENV_HANDLE hEnv)
{
    if(hEnv == NULL)
    {
        return;
    }
	long nLuaResult =  XLLRT_RegisterClass(hEnv,"LuaDir",mLuaDirMemberFunctions,NULL,0);
}

// create instance of LuaDir class
int LuaDirFactory::CreateInstance(lua_State* luaState)
{
	LuaDir *luaDir = new LuaDir(); //ʵ��������
	XLLRT_PushXLObject(luaState,"LuaDir",luaDir);
	return 1;
}

// get instance of LuaDirFactory
LuaDirFactory* __stdcall LuaDirFactory::Instance(void*)
{
    static LuaDirFactory* s_pTheOne = NULL;
    if(s_pTheOne == NULL)
    {
        s_pTheOne = new LuaDirFactory();
    }
    return s_pTheOne;
}

void LuaDirFactory::RegisterObj(XL_LRT_ENV_HANDLE hEnv)
{
    if(hEnv == NULL)
	{
        return ;
	}

    XLLRTObject factoryObject;
	factoryObject.ClassName ="LuaDir.Factory.";
    factoryObject.MemberFunctions = mLuaDirFactoryMemberFunctions;
	factoryObject.ObjName ="LuaDir.Factory.Object";

    factoryObject.userData = NULL;
    factoryObject.pfnGetObject = (fnGetObject)LuaDirFactory::Instance;

    XLLRT_RegisterGlobalObj(hEnv,factoryObject); 
}