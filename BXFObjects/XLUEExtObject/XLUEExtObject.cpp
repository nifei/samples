// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "./XLUEExtObject.h"
#include <XLUE.h>
#include <assert.h>
#include <XLGraphicPlus.h>
#include "MirrorObjectRegister.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

// 扩展模块初始回调，每个扩展模块被加载时候会调用该接口
BOOL XLUE_STDCALL XLUE_InitExtModule()
{
	XLGraphicPlusParam param;
	XLGP_PrepareGraphicPlusParam(&param);

	param.bInitLua = TRUE;
	XLGP_InitGraphicPlus(&param);

	return TRUE;
}

BOOL XLUE_STDCALL XLUE_UninitExtModule()
{
	XLGP_UnInitGraphicPlus();

	return TRUE;
}

BOOL XLUE_STDCALL XLUE_RegisterObj( const char* lpCategory, const char* lpObjClass )
{
	assert(lpCategory);
	assert(lpObjClass);

	BOOL ret = FALSE;

	if (strcmp(lpObjClass, EXTCLASSNAME_MIRROROBJECT) == 0)
	{
		ret = MirrorObjectRegister::RegisterMirrorObject();
	}
	else
		assert(false);

	return ret;
}

BOOL XLUE_STDCALL XLUE_RegisterRes( const char* lpCategory, const char* lpResClass )
{
	assert(lpCategory);
	assert(lpResClass);

	bool ret = false;

	//if (strcmp(lpResClass, XLUE_EXTRES_ICON) == 0)
	//{
	//	ret = IconResRegister::RegisterIconRes();
	//}
	//else
	//{
	//	assert(false);
	//}

	return ret? TRUE : FALSE;
}