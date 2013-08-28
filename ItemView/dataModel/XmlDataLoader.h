#ifndef _XML_DATA_LOADER_H_
#define _XML_DATA_LOADER_H_
#pragma once 

#include "XmlParser.h"
#include <vector>
#include "stdafx.h"
#include "xl_lib/multithread/thread.h"
#include "PostMessageToUIThread.h"

struct StrSongInfo;

typedef void (*funcDataReadyCallback) (DWORD dwUserData1,DWORD dwUserData2,int row, int column);

struct CallbackOnDataReady
{
	funcDataReadyCallback pfnCallback;
	DWORD dwUserData1;
	DWORD dwUserData2;
};

typedef void (*funSingleDataReadyCallback) (int row, StrSongInfo song);

struct CallbackOnDataReadyUnion
{
	MainThreadCallbackFun funCallback;
	void *ptrCaller;
};

// data loader post to ui thread
// uithread call back on data model
struct PostDataBatchMessageToUIThreadUserData
{	
	int from;
	std::vector<StrSongInfo*> list;
	void *ptrCaller;
};

class XmlDataLoader : public xl::win32::multithread::thread
{
public:
	XmlDataLoader();
	~XmlDataLoader();

	bool LoadPlaylist(std::vector<StrSongInfo*> & playlist, const char* fileName);

	XL_BITMAP_HANDLE LoadImage(const char* lpFile);

	bool PrepareData(int from, std::vector<StrSongInfo*> *list);
	bool ReleaseData(int from, std::vector<StrSongInfo*> *list);

	void SetSingleDataReadyListener(DWORD dwUserData1, DWORD dwUserData2, funcDataReadyCallback pfnCallback);
	void SetDataBatchReadyListener(DWORD dwUserData1, DWORD dwUserData2, funcDataReadyCallback pfnCallback);
	void SetDataBatchReadyListener(MainThreadCallbackFun pfnCallback, void* userdata);

	static void UIThreadCallback(void* msg);

protected:
	//thread
	xl::uint32  thread_proc();

private:
	void FireDataReadyEvent(int row, int column){}
	void FireDataBatchReadyEvent(int from, std::vector<StrSongInfo*> playlist);

	XL_BITMAP_HANDLE loadImage( const wchar_t* lpFile );
	XL_BITMAP_HANDLE LoadPng( const wchar_t* lpFile );

	XmlParser *m_parser;
	std::vector<StrSongInfo*> *m_playlist;
	struct range;
	std::vector<range> m_dataRangesWaitingForExecute;
	CallbackOnDataReady *m_callbackOnSingleDataReady;
	CallbackOnDataReadyUnion *m_callbackToDataModelOnDataBatchReady;
	xl::win32::multithread::mutex *m_mutexOnPlaylist;
	xl::win32::multithread::mutex *m_mutexOnRangeList;
};

#endif