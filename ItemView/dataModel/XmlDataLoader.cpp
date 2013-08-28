#include "XmlDataLoader.h"
#include "xl_lib/text/transcode.h"
#include "xl_lib/multithread/mutex.h"
#include "PostMessageToUIThread.h"

struct XmlDataLoader::range
{
	int from;
	int type;
	std::vector<StrSongInfo> *playlist;
	enum{Prepare, Release, Invalid};
	XmlDataLoader::range(int _type)
		:type(_type){}
	XmlDataLoader::range()
		:type(Invalid){}
};

XmlDataLoader::XmlDataLoader()
:m_parser(0), 
m_callbackToDataModelOnDataBatchReady(0),
m_callbackToDataModelOnSingleDataReady(0),
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

bool XmlDataLoader::LoadPlaylist(std::vector<StrSongInfo> & playlist, const char* fileName)
{
	return m_parser->LoadPlaylist(playlist, fileName);
}

/*!
这是一个简单的示例, 在ItemView调用DataModel的PrepareData时, 应该准备加载数据. 
在实际应用中, DataModel在PrepareData是才真正加载数据, 而不是像Demo中这样直接从list中读取.
*/
bool XmlDataLoader::PrepareData(int from, std::vector<StrSongInfo> *list)
{
	m_mutexOnRangeList->lock();
	range r(range::Prepare);
	r.from = from;
	r.playlist = list;
	m_dataRangesWaitingForExecute.push_back(r);
	m_mutexOnRangeList->unlock();
	return true;
}

bool XmlDataLoader::ReleaseData(int from, std::vector<StrSongInfo> *list)
{
	m_mutexOnRangeList->lock();
	range r(range::Release);
	r.from = from;
	r.playlist = list;
	m_dataRangesWaitingForExecute.push_back(r);
	m_mutexOnRangeList->unlock();
	return true;
}

void XmlDataLoader::SetSingleDataReadyListener(MainThreadCallbackFun pfnCallback, void *ptrCaller)
{
	if (m_callbackToDataModelOnSingleDataReady)
		delete m_callbackToDataModelOnSingleDataReady;
	m_callbackToDataModelOnSingleDataReady = new CallbackToDataModelOnDataReady();
	m_callbackToDataModelOnSingleDataReady->funCallback = pfnCallback;
	m_callbackToDataModelOnSingleDataReady->ptrCaller = ptrCaller;
}

void XmlDataLoader::SetDataBatchReadyListener(MainThreadCallbackFun pfnCallback, void* ptrCaller)
{
	if (m_callbackToDataModelOnDataBatchReady)
		delete m_callbackToDataModelOnDataBatchReady;
	m_callbackToDataModelOnDataBatchReady = new CallbackToDataModelOnDataReady();
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
					r.playlist->at(i).hBitmap = LoadImage(r.playlist->at(i).cover.c_str());
					if(PostMessageToUIThread && m_callbackToDataModelOnSingleDataReady)
					{
						PostSingleDataMessageToUIThreadUserData *u = new PostSingleDataMessageToUIThreadUserData();
						u->row = r.from+i;
						u->songInfo = &(r.playlist->at(i));
						u->ptrCaller = m_callbackToDataModelOnSingleDataReady->ptrCaller;
						PostMessageToUIThread((void*)u, m_callbackToDataModelOnSingleDataReady->funCallback);
						//! 此处并没有特别耗资源的操作,为了在界面看到一行一行FireDataReadyEvent()的效果,停顿50msec再操作下一行.
						Sleep(50);
					}
			}
			if (PostMessageToUIThread && m_callbackToDataModelOnDataBatchReady)
			{
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
				if(r.playlist->at(i).hBitmap != NULL)
				{
					XL_ReleaseBitmap(r.playlist->at(i).hBitmap);
					r.playlist->at(i).hBitmap = NULL;
				}
			}
		}
	}// while true
	return 1;
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