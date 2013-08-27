#pragma once
#include "XmlParser.h"
#include "stdafx.h"

struct CallbackOnDataReady;

class XmlDataModel
{
public:
	XmlDataModel(int argc = 0, const char **argv = NULL);
	~XmlDataModel();
	static const char* GetDataModelClassName(){
		return "XmlDataModel";
	}
	static const char* GetDataModelFactoryClassName(){
		return "XmlDataModel.Factory.Class";
	}
	static const char* GetDataModelFactoryObjectName(){
		return "XmlDataModel.Factory";
	}

	char* GetItemAtIndex(int row,int column, const char* view, void **itemData = 0);
	bool GetDataBatch(int from, int to, const char* view, void **dataBatch, char** types);
	int GetCount(const char *view)const;
	int GetColumnCount(const char*view)const;
	void PrepareData(int from, int to, const char* userdata);
	void ReleaseData(int from, int to, const char* userdata);
	void SetSingleDataReadyListener(DWORD dwUserData1, DWORD dwUserData2, funcDataReadyCallback pfnCallback);
	void SetDataBatchReadyListener(DWORD dwUserData1, DWORD dwUserData2, funcDataReadyCallback pfnCallback);
	void FireDataReadyEvent(int row, int column);

private:
	/* 0 is reserved for "this is a song element" tag */
	void initialize();
	CallbackOnDataReady *m_callbackOnDataReady;
	XmlParser *m_parser;
};