#pragma once

#include "XmlDataLoader.h"
#include "LuaDataModelClass.h"
#include "stdafx.h"

class XmlDataModel : public DataModelInterface
{
public:
	XmlDataModel(int argc = 0, const char **argv = NULL);
	virtual ~XmlDataModel();
	virtual char* GetItemAtIndex(int row,int column, void **itemData = 0);
	virtual bool GetDataBatch(int from, int to, void **dataBatch, char** types);
	virtual int GetCount();
	virtual int GetColumnCount();
	virtual void PrepareData(int from, int to);
	virtual void ReleaseData(int from, int to);
	virtual void SetSingleDataReadyListener(DataReadyListenerInterface* dataReadyListener);
	virtual void SetDataBatchReadyListener(DataReadyListenerInterface* dataReadyListener);

	void FireDataReadyEvent(int , int );
	void FireDataReadyEvent(int from, std::vector<StrSongInfo> playlist);
	void FireDataReadyEvent(int row, const StrSongInfo& song);
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
	DataReadyListenerInterface *m_dataReadyListener;
	XmlDataLoader *m_loader;
	std::vector<StrSongInfo> m_playlist;
};