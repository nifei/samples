#pragma once

#include "XmlDataLoader.h"
#include "LuaDataModelClass.h"
#include "stdafx.h"

// call back to LuaDataModelClass info: fun + args
struct CallbackOnDataReady;

// fun type stored in CallbackOnDataReady
typedef void (*funcDataReadyCallback) (DWORD dwUserData1,DWORD dwUserData2,int row, int column);

class XmlDataModel : public DataModelInterface
{
public:
	XmlDataModel(int argc = 0, const char **argv = NULL);
	virtual ~XmlDataModel();
	char* GetItemAtIndex(int row,int column, void **itemData = 0);
	bool GetDataBatch(int from, int to, void **dataBatch, char** types);
	int GetCount()const;
	int GetColumnCount()const;
	void PrepareData(int from, int to);
	void ReleaseData(int from, int to);
	void SetSingleDataReadyListener(DWORD dwUserData1, DWORD dwUserData2, funcDataReadyCallback pfnCallback);
	void SetDataBatchReadyListener(DWORD dwUserData1, DWORD dwUserData2, funcDataReadyCallback pfnCallback);
	void FireDataReadyEvent(int , int );
	void FireDataReadyEvent(int from, std::vector<StrSongInfo> playlist);
	void FireDataReadyEvent(int row, StrSongInfo* song);
	static void UIThreadCallbackOnDataBatch(void *userData);
	static void UIThreadCallbackOnSingleData(void *userData);

	enum {
		MEDIA_TAG = 0,
		COVER_COL = 1,
		NAME_COL = 2,
		SOURCE_COL = 3,
		MAX_COL
	} COL_NAMES;

private:
	CallbackOnDataReady *m_callbackOnDataReady;
	XmlDataLoader *m_loader;
	std::vector<StrSongInfo> m_playlist;
};