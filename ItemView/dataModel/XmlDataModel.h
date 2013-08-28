#pragma once
#include "XmlParser.h"
#include "LuaDataModelClass.h"
#include "stdafx.h"

struct CallbackOnDataReady;

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
	void FireDataReadyEvent(int row, int column);

private:
	/* 0 is reserved for "this is a song element" tag */
	void initialize();
	CallbackOnDataReady *m_callbackOnDataReady;
	XmlParser *m_parser;
};