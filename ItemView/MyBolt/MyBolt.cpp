// HelloBolt.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include <XLUE.h>
#include <XLGraphic.h>
#include <XLLuaRuntime.h>
#include <string>
#include <vector>
#include <Shlwapi.h>
#include <assert.h>
#include <windows.h>
#include "datamodel.h"
using namespace std;

const WCHAR* GetResDir()
{
    static WCHAR wszModulePath[MAX_PATH];
    GetModuleFileNameW(NULL,wszModulePath,MAX_PATH);
	PathAppend(wszModulePath, L"..\\..\\samples\\ItemView\\XAR");
    return wszModulePath;
}

int __stdcall LuaErrorHandle(lua_State* luaState,const wchar_t* pExtInfo,const wchar_t* wcszLuaErrorString,PXL_LRT_ERROR_STACK pStackInfo)
{
    static bool s_bEnter = false;
    if (!s_bEnter)
    {
        s_bEnter = true;
        if(pExtInfo != NULL)
        {
			wstring str = wcszLuaErrorString ? wcszLuaErrorString : L"";
            luaState;
            pExtInfo;
            wcszLuaErrorString;
            str += L" @ ";
            str += pExtInfo;

            MessageBoxW(0,str.c_str(),L"Ϊ�˰������ǸĽ�����,�뷴���˽ű�����",MB_ICONERROR | MB_OK);

        }
        else
        {
			MessageBoxW(0,wcszLuaErrorString ? wcszLuaErrorString : L"" ,L"Ϊ�˰������ǸĽ�����,�뷴���˽ű�����",MB_ICONERROR | MB_OK);
        }
        s_bEnter = false;
    }	
    return 0;
}

bool InitXLUE()
{
    //��ʼ��ͼ�ο�
    XLGraphicParam param;
    XL_PrepareGraphicParam(&param);
	long result = XL_InitGraphicLib(&param);
    //��ʼ��XLUE,�⺯����һ�����ϳ�ʼ������
    //����˳�ʼ��Lua����,��׼����,XLUELoader�Ĺ���
	param.textType = XLTEXT_TYPE_FREETYPE;
	result = XL_SetFreeTypeEnabled(TRUE);
    result = XLUE_InitLoader(NULL);

	//����һ���򵥵Ľű�������ʾ
    XLLRT_ErrorHandle(LuaErrorHandle);
    return true; 
}

void UninitXLUE()
{
    //�˳�����
    XLUE_Uninit(NULL);
    XLUE_UninitLuaHost(NULL);
    XL_UnInitGraphicLib();
    XLUE_UninitHandleMap(NULL);
}

bool LoadMainXAR()
{
    long result = 0;
    //����XAR������·��
    result = XLUE_AddXARSearchPath(GetResDir());
    //������XAR,��ʱ��ִ�и�XAR�������ű�onload.lua
    result = XLUE_LoadXAR("MyBolt");
    if(result != 0)
    {
        return false;
    }
    return true;
}

bool RegisterLogClass()
{
	HINSTANCE hmod;
	hmod =::LoadLibrary(L"log.dll");
	typedef bool (*Fun)(void);
	Fun lpproc = (Fun)GetProcAddress (hmod,"RegisterLogClass");
	if (lpproc != 0)
		return lpproc();
	return false;
}

bool RegisterDataModelClass()
{
	HINSTANCE hmod;
	hmod =::LoadLibrary(L"dataModel.dll");
	typedef bool (*Fun)(void);
	Fun lpproc = (Fun)GetProcAddress (hmod,"RegisterDataModelClass");
	if (lpproc != 0)
		return lpproc();
	return false;
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: �ڴ˷��ô��롣
	if(!InitXLUE())
    {
        MessageBoxW(NULL,L"��ʼ��XLUE ʧ��!",L"����",MB_OK);
        return 1;
    }

	RegisterLogClass();
	RegisterDataModelClass();

    if(!LoadMainXAR())
    {
        MessageBoxW(NULL,L"Load XARʧ��!",L"����",MB_OK);
        return 1;
    }

	MSG msg;
	
	// ����Ϣѭ��:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	UninitXLUE();

	return (int) msg.wParam;
}
