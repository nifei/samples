#include "stdafx.h"
#include "XmlDataModel.h"
#include "datamodel.h"
#include "./LuaDataModelClass.h"

bool RegisterDataModelClass()
{
	XL_LRT_ENV_HANDLE hEnv = XLLRT_GetEnv(NULL);
	LuaDataModelClass::RegisterClass("XmlDataModel", hEnv);
	LuaDataModelClassFactory::RegisterObj("XmlDataModel", hEnv);
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