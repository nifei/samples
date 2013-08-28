#ifndef _XML_DATA_LOADER_H_
#define _XML_DATA_LOADER_H_
#pragma once 

#include "XmlParser.h"
#include <vector>
#include "stdafx.h"
#include "xl_lib/multithread/thread.h"
#include "PostMessageToUIThread.h"

struct StrSongInfo;

// when data is ready, data model expected to be called through ptrCaller->funCallback()
// no distinguish between databatch ready or single data ready, data model class assign different funCallback when set listener
struct CallbackToDataModelOnDataReady
{
	MainThreadCallbackFun funCallback;
	void *ptrCaller;
};

// data loader post to ui thread
// uithread call back on data model
struct PostDataBatchMessageToUIThreadUserData
{	
	int from;
	std::vector<StrSongInfo> list;
	void *ptrCaller;
};

struct PostSingleDataMessageToUIThreadUserData
{
	int row;
	StrSongInfo *songInfo;
	void *ptrCaller;
};

class XmlDataLoader : public xl::win32::multithread::thread
{
public:
	XmlDataLoader();
	~XmlDataLoader();

	bool LoadPlaylist(std::vector<StrSongInfo> & playlist, const char* fileName);

	XL_BITMAP_HANDLE LoadImage(const char* lpFile);

	bool PrepareData(int from, std::vector<StrSongInfo> *list);
	bool ReleaseData(int from, std::vector<StrSongInfo> *list);
	void SetDataBatchReadyListener(MainThreadCallbackFun pfnCallback, void* userdata);
	void SetSingleDataReadyListener(MainThreadCallbackFun pfnCallback, void* userdata);

protected:
	//thread
	xl::uint32  thread_proc();

private:
	XL_BITMAP_HANDLE loadImage( const wchar_t* lpFile );
	XL_BITMAP_HANDLE LoadPng( const wchar_t* lpFile );

	XmlParser *m_parser;
	std::vector<StrSongInfo> *m_playlist;
	struct range;
	std::vector<range> m_dataRangesWaitingForExecute;
	CallbackToDataModelOnDataReady *m_callbackToDataModelOnDataBatchReady;
	CallbackToDataModelOnDataReady *m_callbackToDataModelOnSingleDataReady;
	xl::win32::multithread::mutex *m_mutexOnPlaylist;
	xl::win32::multithread::mutex *m_mutexOnRangeList;
};

#endif