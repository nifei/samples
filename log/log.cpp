// log.cpp : Defines the entry point for the DLL application.
// ����һ��û�лص�����ı�lua���õ���򵥵�dll��ʵ��

#include "stdafx.h"
#include "./LuaLogClass.h"
#include "log.h"

bool RegisterLogClass()
{
	XL_LRT_ENV_HANDLE hEnv = XLLRT_GetEnv(NULL);
	LuaLogClass::RegisterClass(hEnv);
	LuaLogClassFactory::RegisterObj(hEnv);
	XLLRT_ReleaseEnv(hEnv);
	return true;
}

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
   return TRUE;
}

__declspec(dllexport) void NoUse()
{

}