#ifndef _XML_PARSER_H_
#define _XML_PARSER_H_
#pragma once 

#include <xl_lib/xml/expat_parser.h>
#include <vector>
#include "stdafx.h"
#include "xl_lib/multithread/thread.h"
#include "xl_lib/multithread/mutex.h"

struct StrSongInfo;

typedef void (*funcDataReadyCallback) (DWORD dwUserData1,DWORD dwUserData2,int row, int column);

struct CallbackOnDataReady
{
	funcDataReadyCallback pfnCallback;
	DWORD dwUserData1;
	DWORD dwUserData2;
};

class XmlParser : public xl::xml::expat_parser<XmlParser>, public xl::win32::multithread::thread
{
public:
	XmlParser(wchar_t* fileName);
	XmlParser(char* fileName);
	~XmlParser();
	bool GetDataBatch(int from, int to, void** dataBatch, char** types = NULL);
	bool GetDataAtIndex(int row, int column, void **itemData, char **type=NULL);
	int GetCount()const {return m_playlist->size();}
	int GetColumnCount()const{return MAX_COL-1;}
	bool PrepareData(int from, int to);
	bool ReleaseData(int from, int to);
	void RunInMainThread();
	void AttachSingleDataReadyListener(DWORD dwUserData1, DWORD dwUserData2, funcDataReadyCallback pfnCallback);
	void AttachDataBatchReadyListener(DWORD dwUserData1, DWORD dwUserData2, funcDataReadyCallback pfnCallback);
	static void UIThreadCallback(void* msg);
public:
	// expat_parser methods
	void OnPostCreate();
	void OnStartElement (const XML_Char *pszName, const XML_Char **papszAttrs);
	void OnEndElement (const XML_Char *);

	enum {
		MEDIA_TAG = 0,
		COVER_COL = 1,
		NAME_COL = 2,
		SOURCE_COL = 3,
		MAX_COL
	} COL_NAMES;

protected:
	//thread
	xl::uint32  thread_proc();

private:
	void init();
	void FireDataReadyEvent(int row, int column);
	void FireDataBatchReadyEvent(int from, int to);
	bool loadPlaylist();
	XL_BITMAP_HANDLE loadImage( const wchar_t* lpFile );
	XL_BITMAP_HANDLE loadImage(const char* lpFile);
	XL_BITMAP_HANDLE LoadPng( const wchar_t* lpFile );

	char * attrNamesOfColumn[MAX_COL];
	struct range;
	std::vector<range> m_dataRangesWaitingForExecute;
	wchar_t* m_playlistName;
	std::vector<StrSongInfo*> *m_playlist;
	CallbackOnDataReady *m_callbackOnSingleDataReady;
	CallbackOnDataReady *m_callbackOnDataBatchReady;
	xl::win32::multithread::mutex *m_mutexOnPlaylist;
	xl::win32::multithread::mutex *m_mutexOnRangeList;
};

#endif