// log.cpp : Defines the entry point for the DLL application.
// 这是一个没有回调需求的被lua调用的最简单的dll的实现

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