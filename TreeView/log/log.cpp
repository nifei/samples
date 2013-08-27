// log.cpp : Defines the entry point for the DLL application.
// ����һ��û�лص�����ı�lua���õ���򵥵�dll��ʵ��

#include "stdafx.h"
#include "./LuaLogClass.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    if(ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        XL_LRT_ENV_HANDLE hEnv = XLLRT_GetEnv(NULL);
        LuaLogClass::RegisterClass(hEnv);
        LuaLogClassFactory::RegisterObj(hEnv);
    }

    return TRUE;
}

__declspec(dllexport) void NoUse()
{

}