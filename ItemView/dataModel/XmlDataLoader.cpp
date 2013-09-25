#include "XmlDataLoader.h"
#include "xl_lib/text/transcode.h"
#include "PostMessageToUIThread.h"

struct XmlDataLoader::range
{
	int from;
	int type;
	std::vector<StrSongInfo> *playlist;
	enum{Prepare, Release, Invalid};
	XmlDataLoader::range(int _type)
		:playlist(NULL), type(_type){}
	XmlDataLoader::range()
		:playlist(NULL), type(Invalid){}
	~range(){
	}
};

XmlDataLoader::XmlDataLoader()
:m_parser(NULL), 
m_callbackToDataModelOnDataBatchReady(NULL),
m_callbackToDataModelOnSingleDataReady(NULL)
{
	m_parser = new XmlParser();
	range r;
	r.type = range::Invalid;
	r.playlist = new std::vector<StrSongInfo>();
	r.playlist->push_back(StrSongInfo());
	m_dataRangesWaitingForExecute.push_back(r);
}

XmlDataLoader::~XmlDataLoader()
{
	m_event.set_event();
	stop();
	if (m_parser)
	{
		delete m_parser;
		m_parser = NULL;
	}
	if (m_callbackToDataModelOnDataBatchReady)
	{
		delete m_callbackToDataModelOnDataBatchReady;
		m_callbackToDataModelOnDataBatchReady = NULL;
	}
	if (m_callbackToDataModelOnSingleDataReady)
	{
		delete m_callbackToDataModelOnSingleDataReady;
		m_callbackToDataModelOnSingleDataReady = NULL;
	}
	//m_dataRangesWaitingForExecute.clear();
	/*
		为什么需要遍历销毁waitingForExecute的range中的playlist:
		clear()会调用range的析构方法, 但是range的指针成员playlist不会被销毁, 只会销毁对象
		为什么不可以在range的析构方法中销毁playlist:
		range是作为对象传递的, 离开作用域就会被销毁
	*/
	while(!m_dataRangesWaitingForExecute.empty())
	{
		range r = m_dataRangesWaitingForExecute.front();
		m_dataRangesWaitingForExecute.erase(m_dataRangesWaitingForExecute.begin());
		delete r.playlist;
	}
}

bool XmlDataLoader::LoadPlaylist(std::vector<StrSongInfo> & playlist, const char* fileName)
{
	return m_parser->LoadPlaylist(playlist, fileName);
}

/*!
这是一个简单的示例, 在ItemView调用DataModel的PrepareData时, 应该准备加载数据. 
在实际应用中, DataModel在PrepareData是才真正加载数据, 而不是像Demo中这样直接从list中读取.
*/
bool XmlDataLoader::PrepareData(int from, int to, const std::vector<StrSongInfo>& allList)
{
	std::vector<StrSongInfo> *playlist = new std::vector<StrSongInfo>(to-from+1);
	std::vector<StrSongInfo>::const_iterator it = allList.begin();
	std::vector<StrSongInfo>::const_iterator itBegin = it + from-1;
	std::vector<StrSongInfo>::const_iterator itEnd = it + to;//最后一个不包括在内
	playlist->assign(itBegin, itEnd);

	range r(range::Prepare);
	r.from = from;
	r.playlist = playlist;

	appendRange(r);
	return true;
}

bool XmlDataLoader::ReleaseData(int from, int to, const std::vector<StrSongInfo> &allList)
{
	std::vector<StrSongInfo> *list = new std::vector<StrSongInfo>(to-from+1);
	std::vector<StrSongInfo>::const_iterator it = allList.begin();
	std::vector<StrSongInfo>::const_iterator itBegin = it + from-1;
	std::vector<StrSongInfo>::const_iterator itEnd = it + to;//最后一个不包括在内
	list->assign(itBegin, itEnd);
	range r(range::Release);
	r.from = from;
	r.playlist = list;

	appendRange(r);
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
	while(m_event.wait_event())
	{
		if (this->is_stop_event_set())
			break;
		range r;	
		r.type = range::Invalid;
		getRange(r);
		if (r.type == range::Prepare)
		{
			for (int i = 0; i < r.playlist->size(); i++)	
			{
					r.playlist->at(i).hBitmap = LoadImage(r.playlist->at(i).cover.c_str());
					if(PostMessageToUIThread && m_callbackToDataModelOnSingleDataReady)
					{
						//工作线程创建, UI线程销毁?
						PostSingleDataMessageToUIThreadUserData *u = new PostSingleDataMessageToUIThreadUserData();
						u->row = r.from+i;
						u->song = r.playlist->at(i);
						u->ptrCaller = m_callbackToDataModelOnSingleDataReady->ptrCaller;
						PostMessageToUIThread((void*)u, m_callbackToDataModelOnSingleDataReady->funCallback);
						//! 此处并没有特别耗资源的操作,为了在界面看到一行一行FireDataReadyEvent()的效果,停顿10msec再操作下一行.
						Sleep(10);
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
		if (r.playlist)
		{
			delete r.playlist;
		}
	}
	return 1;
}

bool XmlDataLoader::appendRange(XmlDataLoader::range r)
{
	m_cs.lock();
	m_dataRangesWaitingForExecute.push_back(r);
	if (m_dataRangesWaitingForExecute.size() == 1)
	{
		m_event.set_event();
	}
	m_cs.unlock();
	return true;
}

bool XmlDataLoader::getRange(XmlDataLoader::range &r)
{
	m_cs.lock();
	if (m_dataRangesWaitingForExecute.empty() == false)
	{
		r = m_dataRangesWaitingForExecute.front();
		m_dataRangesWaitingForExecute.erase(m_dataRangesWaitingForExecute.begin());
		if (m_dataRangesWaitingForExecute.empty())
		{
			m_event.reset_event();
		}
	}
	m_cs.unlock();
	return true;
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
	xl::text::transcode::UTF8_to_Unicode(lpFile, MAX_PATH, wlpFile);
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