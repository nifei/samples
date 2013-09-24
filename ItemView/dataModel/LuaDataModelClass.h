#pragma once 
#ifndef _LUA_DATAMODEL_CLASS_H_
#define _LUA_DATAMODEL_CLASS_H_
#include "stdafx.h"

typedef void (*funcDataReadyCallback) (DWORD dwUserData1,DWORD dwUserData2,int row, int column);

class DataModelInterface
{
public:
	virtual ~DataModelInterface() {}
	virtual char* GetItemAtIndex(int row,int column, void **itemData = 0) = 0;
	virtual int GetCount()const = 0;
	virtual int GetColumnCount()const = 0;
	virtual bool GetDataBatch(int from, int to, void **dataBatch, char** types) {return false;}
	virtual void PrepareData(int from, int to) {}
	virtual void ReleaseData(int from, int to) {}
	virtual void SetSingleDataReadyListener(DWORD dwUserData1, DWORD dwUserData2, funcDataReadyCallback pfnCallback) {}
	virtual void SetDataBatchReadyListener(DWORD dwUserData1, DWORD dwUserData2, funcDataReadyCallback pfnCallback) {}
};

class LuaDataModelClassFactory
{
public:
	static LuaDataModelClassFactory * __stdcall Instance(void*);
	/*!
		��Ҫ��������DataModel��������lua������һ������, Ҳ����luaջ�ϵڶ���λ��
	*/
	static int CreateInstance(lua_State *luaState);
	/*!
		@param dataModelClassName�����Ҫ��ע���DataModel������, ע��ʱ���ֱ����.factory. class��.factory.objectע�ᵽlua������
	*/
	static void RegisterObj(const char *dataModelClassName, XL_LRT_ENV_HANDLE hEnv);
private:
	static XLLRTGlobalAPI mLuaDataModelClassFactoryMemberFunctions[];
};

class LuaDataModelClass
{
public:
	static int DeleteSelf(lua_State *luaState);
	static int GetItemAtIndex(lua_State *luaState);
	static int GetDataBatch(lua_State *luaState);
	static int GetCount(lua_State *luaState);
	static int GetColumnCount(lua_State *luaState);
	static int PrepareData(lua_State *luaState);
	static int ReleaseData(lua_State *luaState);
	/*!
		����û��ṩ��ItemView��DataModelCallbackTable�а���SetDataBatchReadyListener, ��ôItemView����ø÷�������LuaDataModelClassһ��Lua�ص�����DataReadyCallback������׼���õ�ʱ��֪ͨItemView
		LuaDataModelClass���DataReadyCallback��ͬ�ص�ʱ��Ҫ�Ĳ���dwUserData1, dwUserData2ԭ�ⲻ���ش���DataModel, ��DataModel׼�������ݵ�ʱ��Ӧ�õ���DataReadyCallback. ����DataModel��Ӧ��ֱ�Ӳ���lua�ӿ�, ���Գ���lua����DataReadyCallback��dwUserData1, dwUserData2֮��, LuaDataModelClass����Լ��ķ���LuaDataModelClass::LuaDataReadyListenerҲ��Ϊ��������DataModel, DataModel�ص��������֪ͨ����׼�����˾Ϳ�����. 
		///sa AttachDataReadyListener
	*/
	static int SetDataBatchReadyListener(lua_State *luaState);
	/*!
		���������LuaDataModelClass::SetDataBatchReadyListener���÷���һ����. ��֮ͬ������SetDataBatchReadyListener�Ļص�����DataBatchReady����һ������׼���õ�ʱ�򱻵���. ����ItemView����PrepareData(1, 100), ��ô�����100������׼�����Ժ�, DataModelӦ�õ���DataBatchReady��֪ͨItemView��һ�������Ѿ����Ա�������. ��SetSingleDataReadyListener����Ļص�����Ӧ����ÿһ����Ԫ������ݾ���֮�󶼱�����һ��, �ص����Ĳ����Ǿ�����Ԫ���������
	*/
	static int SetSingleDataReadyListener(lua_State *luaState);
	static void RegisterClass(const char * dataModelClassName, XL_LRT_ENV_HANDLE hEnv);
	static void LuaDataReadyListener(DWORD dwUserData1, DWORD dwUserData2, int row, int column);
private:
	static DataModelInterface *GetDataModelClassObject(lua_State *luaState);
	static XLLRTGlobalAPI mLuaDataModelClassMemberFunctions[];
};

#endif