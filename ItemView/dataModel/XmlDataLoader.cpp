#include "XmlDataLoader.h"
#include "xl_lib/text/transcode.h"
#include "xl_lib/multithread/mutex.h"
#include "PostMessageToUIThread.h"

struct XmlDataLoader::range
{
	int from;
	int type;
	std::vector<StrSongInfo*> *playlist;
	enum{Prepare, Release, Invalid};
	XmlDataLoader::range(int _type)
		:type(_type){}
	XmlDataLoader::range()
		:type(Invalid){}
};

struct XmlPostUIThreadUserData
{
	enum{SingleDataReady, DataBatchReady};
	int type;
	XmlDataLoader *loader;
	int arg1;
	int arg2;
	std::vector<StrSongInfo *> playlist;
	XmlPostUIThreadUserData(XmlDataLoader *_loader, int _arg1, int _arg2, int _type)
		:loader(_loader), arg1(_arg1), arg2(_arg2), type(_type){}
};


XmlDataLoader::XmlDataLoader()
:m_parser(0), 
m_callbackOnSingleDataReady(0),
m_callbackToDataModelOnDataBatchReady(0),
m_mutexOnPlaylist(0), 
m_mutexOnRangeList(0)
{
	InitUIThread();
	m_parser = new XmlParser();
	m_mutexOnPlaylist = new xl::win32::multithread::mutex(false, L"PlaylistMutex");
	m_mutexOnRangeList= new xl::win32::multithread::mutex(false, L"RangeListMutex");
}

XmlDataLoader::~XmlDataLoader()
{
	if (m_parser)
	{
		delete m_parser;
		m_parser = 0;
	}
	UnInitUIThread();
}

bool XmlDataLoader::LoadPlaylist(std::vector<StrSongInfo*> & playlist, const char* fileName)
{
	return m_parser->LoadPlaylist(playlist, fileName);
}

/*!
这是一个简单的示例, 在ItemView调用DataModel的PrepareData时, 应该准备加载数据. 
在实际应用中, DataModel在PrepareData是才真正加载数据, 而不是像Demo中这样直接从list中读取.
*/
bool XmlDataLoader::PrepareData(int from, std::vector<StrSongInfo*> *list)
{
	m_mutexOnRangeList->lock();
	range r(range::Prepare);
	r.from = from;
	r.playlist = list;
	m_dataRangesWaitingForExecute.push_back(r);
	m_mutexOnRangeList->unlock();
	return true;
}

/*!
注意: PrepareData和ReleaseData要在同一线程中操作m_playlist; 否则会有问题
*/
bool XmlDataLoader::ReleaseData(int from, std::vector<StrSongInfo*> *list)
{
	m_mutexOnRangeList->lock();
	range r(range::Release);
	r.from = from;
	r.playlist = list;
	m_dataRangesWaitingForExecute.push_back(r);
	m_mutexOnRangeList->unlock();
	return true;
}

void XmlDataLoader::SetSingleDataReadyListener(DWORD dwUserData1, DWORD dwUserData2, funcDataReadyCallback pfnCallback)
{
	if (m_callbackOnSingleDataReady)
		delete m_callbackOnSingleDataReady;
	m_callbackOnSingleDataReady = new CallbackOnDataReady();
	m_callbackOnSingleDataReady->dwUserData1 = dwUserData1;
	m_callbackOnSingleDataReady->dwUserData2 = dwUserData2;
	m_callbackOnSingleDataReady->pfnCallback = pfnCallback;
}

void XmlDataLoader::SetDataBatchReadyListener(MainThreadCallbackFun pfnCallback, void* ptrCaller)
{
	if (m_callbackToDataModelOnDataBatchReady)
		delete m_callbackToDataModelOnDataBatchReady;
	m_callbackToDataModelOnDataBatchReady = new CallbackOnDataReadyUnion();
	m_callbackToDataModelOnDataBatchReady->funCallback = pfnCallback;
	m_callbackToDataModelOnDataBatchReady->ptrCaller = ptrCaller;
}

xl::uint32  XmlDataLoader::thread_proc()
{
	while(true)
	{
		range r;
		r.type = range::Invalid;
		if (m_mutexOnRangeList->try_lock(1000))
		{
			if (m_dataRangesWaitingForExecute.empty()==false)
			{
				r= m_dataRangesWaitingForExecute.front();
				m_dataRangesWaitingForExecute.erase(m_dataRangesWaitingForExecute.begin());
			}
			m_mutexOnRangeList->unlock();
		}
		if (r.type == range::Prepare)
		{
			for (int i = 0; i < r.playlist->size(); i++)
			{
					r.playlist->at(i)->hBitmap = LoadImage(r.playlist->at(i)->cover.c_str());
					if(PostMessageToUIThread && m_callbackOnSingleDataReady)
					{
						XmlPostUIThreadUserData *u1 = new XmlPostUIThreadUserData(this, r.from+i, 1, XmlPostUIThreadUserData::SingleDataReady);
						u1->playlist.push_back(r.playlist->at(i));
						XmlPostUIThreadUserData *u2 = new XmlPostUIThreadUserData(this, r.from+i, 2, XmlPostUIThreadUserData::SingleDataReady);
						u2->playlist.push_back(r.playlist->at(i));
						XmlPostUIThreadUserData *u3 = new XmlPostUIThreadUserData(this, r.from+i, 3, XmlPostUIThreadUserData::SingleDataReady);
						u3->playlist.push_back(r.playlist->at(i));
						PostMessageToUIThread((void*)u1, XmlDataLoader::UIThreadCallback);
						PostMessageToUIThread((void*)u2, XmlDataLoader::UIThreadCallback);
						PostMessageToUIThread((void*)u3, XmlDataLoader::UIThreadCallback);
						//! 此处并没有特别耗资源的操作,为了在界面看到一行一行FireDataReadyEvent()的效果,停顿50msec再操作下一行.
						Sleep(50);
					}
			}
			if (PostMessageToUIThread && m_callbackToDataModelOnDataBatchReady)
			{
				//int to = r.from + r.playlist->size()-1;
				//XmlPostUIThreadUserData *u = new XmlPostUIThreadUserData(this, r.from, to, XmlPostUIThreadUserData::DataBatchReady);
				//u->playlist.assign(r.playlist->begin(), r.playlist->end());
				//PostMessageToUIThread((void*)u, XmlDataLoader::UIThreadCallback);
				PostDataBatchMessageToUIThreadUserData *u = new PostDataBatchMessageToUIThreadUserData();
				u->list.assign(r.playlist->begin(), r.playlist->end());
				u->from = r.from;
				u->ptrCaller = m_callbackToDataModelOnDataBatchReady->ptrCaller;
				PostMessageToUIThread((void*)u, m_callbackToDataModelOnDataBatchReady->funCallback);
			}
		} else if (r.type == range::Release)
		{
			for (int i = 0; i < r.playlist->size(); i++)
			{
				if(r.playlist->at(i)->hBitmap != NULL)
				{
					XL_ReleaseBitmap(r.playlist->at(i)->hBitmap);
					r.playlist->at(i)->hBitmap = NULL;
				}
			}
		}
	}// while true
	return 1;
}

void XmlDataLoader::UIThreadCallback(void* _userData)
{
	if (XmlPostUIThreadUserData *userData = (XmlPostUIThreadUserData*)_userData)
	{
		if (userData->type == XmlPostUIThreadUserData::SingleDataReady)
		{
			XmlDataLoader *loader = userData->loader;
			int row = userData->arg1;
			int column = userData->arg2;
			loader->FireDataReadyEvent(row, column);
		} else if (userData->type == XmlPostUIThreadUserData::DataBatchReady)
		{
			XmlDataLoader *loader = userData->loader;
			int from = userData->arg1;
			int to = userData->arg2;
			loader->FireDataBatchReadyEvent(from, userData->playlist);
		}
		delete _userData;
	}
}

void XmlDataLoader::FireDataBatchReadyEvent(int from, std::vector<StrSongInfo*> playlist)
{
}

XL_BITMAP_HANDLE XmlDataLoader::LoadPng( const wchar_t* lpFile )
{
	assert(lpFile);
	XL_BITMAP_HANDLE hBitmap = XL_LoadBitmapFromFile(lpFile, XLGRAPHIC_CT_ARGB32);
	assert(hBitmap);
	return hBitmap;
}

XL_BITMAP_HANDLE XmlDataLoader::LoadImage(const char* lpFile)
{
	std::wstring wlpFile;
	xl::text::transcode::UTF8_to_Unicode(lpFile, strlen(lpFile), wlpFile);
	return loadImage(wlpFile.c_str());
}

XL_BITMAP_HANDLE XmlDataLoader::loadImage( const wchar_t* lpFile )
{
	const wchar_t* lpExt = ::PathFindExtension(lpFile);
	if (lpExt && ::lstrcmpi(lpExt, L".png") == 0)
	{
		return LoadPng(lpFile); 
	}
	return NULL;
}