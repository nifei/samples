#include "stdafx.h"
#include "./LuaDataModelClass.h"
#include "XmlDataModel.h"
#include "datamodel.h"

bool RegisterDataModelClass()
{
	XL_LRT_ENV_HANDLE hEnv = XLLRT_GetEnv(NULL);
	LuaDataModelClass<XmlDataModel>::RegisterClass(hEnv);
	LuaDataModelClassFactory<XmlDataModel>::RegisterObj(hEnv);
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