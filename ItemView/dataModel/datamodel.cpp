#include "stdafx.h"
#include "./LuaDataModelClass.h"
#include "XmlDataModel.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    if(ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        XL_LRT_ENV_HANDLE hEnv = XLLRT_GetEnv(NULL);
		LuaDataModelClass<XmlDataModel>::RegisterClass(hEnv);
        LuaDataModelClassFactory<XmlDataModel>::RegisterObj(hEnv);
    }
    return TRUE;
}

__declspec(dllexport) void NoUse()
{

}