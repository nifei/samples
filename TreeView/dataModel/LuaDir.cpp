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
		wchar_t szLogicalDriveStrings[MAX_PATH];                          //获取驱动器的内存
		wchar_t *szDrive;
		::ZeroMemory(szLogicalDriveStrings,MAX_PATH);                    //将内存清零，第一个参数是申请字符的地址
		::GetLogicalDriveStrings(MAX_PATH-1,szLogicalDriveStrings);      //获取磁盘中的所有驱动器
		szDrive=(wchar_t*)szLogicalDriveStrings;

		lua_newtable(L);
		int index = 1;

		do
		{
			int len = wcslen(szDrive);
			std::string atemp;
			xl::text::transcode::Unicode_to_ANSI(szDrive, MAX_PATH, atemp);
			const char* volume = atemp.c_str();
			UINT uDriveType = GetDriveType(szDrive);
			if (DRIVE_UNKNOWN == uDriveType)
			{
			}

			lua_newtable(L);
			lua_pushnumber(L, 1);
			lua_pushboolean(L, true);
			lua_settable(L, -3);

			lua_pushnumber(L, 2);
			lua_pushstring(L, volume);
			lua_settable(L, -3);

			lua_pushnumber(L, index);
			lua_insert(L, -2); // 把栈顶的一维索引和rowtable换一下位置, 
			lua_settable(L, -3); // 把刚压栈的一维索引row, 和rowtable弹出, 放在要返回的return table中
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

				wchar_t subDir[MAX_PATH];
				wcscpy(subDir, fdFile.cFileName);
				std::string atemp;
				xl::text::transcode::Unicode_to_ANSI(fdFile.cFileName, MAX_PATH, atemp);
				const char* aSubDir = atemp.c_str();

				lua_pushnumber(L, 2);
				lua_pushstring(L, aSubDir);
				lua_settable(L, -3);
				
				lua_pushnumber(L, index);//把一维索引row压栈
				lua_insert(L, -2); // 把栈顶的一维索引和rowtable换一下位置, 
				lua_settable(L, -3); // 把刚压栈的一维索引row, 和rowtable弹出, 放在要返回的return table中
				index++;
			}
		} while(::FindNextFile(hFind, &fdFile));
		::FindClose(hFind);
	}
	return;
}

XLLRTGlobalAPI LuaDir::mLuaDirMemberFunctions[] = 
{
    {"__gc",LuaDir::DeleteSelf},
	{"GetSubDirs", LuaDir::GetSubDirs},
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
	LuaDir *luaDir = new LuaDir(); //实例化问题
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