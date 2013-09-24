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
		需要被创建的DataModel类名称在lua函数第一个参数, 也就是lua栈上第二个位置
	*/
	static int CreateInstance(lua_State *luaState);
	/*!
		@param dataModelClassName是这个要被注册的DataModel的类名, 注册时候会分别加上.factory. class和.factory.object注册到lua环境中
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
		如果用户提供给ItemView的DataModelCallbackTable中包含SetDataBatchReadyListener, 那么ItemView会调用该方法传给LuaDataModelClass一个Lua回调方法DataReadyCallback在数据准备好的时候通知ItemView
		LuaDataModelClass会把DataReadyCallback连同回调时需要的参数dwUserData1, dwUserData2原封不动地传给DataModel, 当DataModel准备好数据的时候应该调用DataReadyCallback. 但是DataModel不应该直接操作lua接口, 所以除了lua方法DataReadyCallback和dwUserData1, dwUserData2之外, LuaDataModelClass会把自己的方法LuaDataModelClass::LuaDataReadyListener也作为参数传给DataModel, DataModel回调这个方法通知数据准备好了就可以了. 
		///sa AttachDataReadyListener
	*/
	static int SetDataBatchReadyListener(lua_State *luaState);
	/*!
		这个方法和LuaDataModelClass::SetDataBatchReadyListener的用法是一样的. 不同之处在于SetDataBatchReadyListener的回调方法DataBatchReady会在一批数据准备好的时候被调用. 例如ItemView调用PrepareData(1, 100), 那么在这个100个数据准备好以后, DataModel应该调用DataBatchReady来通知ItemView有一批数据已经可以被调用了. 而SetSingleDataReadyListener传入的回调方法应该在每一个单元格的数据就绪之后都被调用一次, 回调传的参数是就绪单元格的行与列
	*/
	static int SetSingleDataReadyListener(lua_State *luaState);
	static void RegisterClass(const char * dataModelClassName, XL_LRT_ENV_HANDLE hEnv);
	static void LuaDataReadyListener(DWORD dwUserData1, DWORD dwUserData2, int row, int column);
private:
	static DataModelInterface *GetDataModelClassObject(lua_State *luaState);
	static XLLRTGlobalAPI mLuaDataModelClassMemberFunctions[];
};

#endif