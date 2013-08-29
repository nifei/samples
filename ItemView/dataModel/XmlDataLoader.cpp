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
		:playlist(0), type(_type){}
	XmlDataLoader::range()
		:playlist(0), type(Invalid){}
};

xl::win32::multithread::critical_section XmlDataLoader::m_cs;

XmlDataLoader::XmlDataLoader()
:m_parser(0), 
m_callbackToDataModelOnDataBatchReady(0),
m_callbackToDataModelOnSingleDataReady(0)
{
	m_parser = new XmlParser();
}

XmlDataLoader::~XmlDataLoader()
{
	if (m_parser)
	{
		delete m_parser;
		m_parser = 0;
	}
}

bool XmlDataLoader::LoadPlaylist(std::vector<StrSongInfo> & playlist, const char* fileName)
{
	return m_parser->LoadPlaylist(playlist, fileName);
}

/*!
����һ���򵥵�ʾ��, ��ItemView����DataModel��PrepareDataʱ, Ӧ��׼����������. 
��ʵ��Ӧ����, DataModel��PrepareData�ǲ�������������, ��������Demo������ֱ�Ӵ�list�ж�ȡ.
*/
bool XmlDataLoader::PrepareData(int from, int to, const std::vector<StrSongInfo>& allList)
{
	std::vector<StrSongInfo> *playlist = new std::vector<StrSongInfo>(to-from+1);
	std::vector<StrSongInfo>::const_iterator it = allList.begin();
	std::vector<StrSongInfo>::const_iterator itBegin = it + from-1;
	std::vector<StrSongInfo>::const_iterator itEnd = it + to;//���һ������������
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
	std::vector<StrSongInfo>::const_iterator itEnd = it + to;//���һ������������
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
	while(true)
	{
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
						PostSingleDataMessageToUIThreadUserData *u = new PostSingleDataMessageToUIThreadUserData();
						u->row = r.from+i;
						u->song = r.playlist->at(i);
						u->ptrCaller = m_callbackToDataModelOnSingleDataReady->ptrCaller;
						PostMessageToUIThread((void*)u, m_callbackToDataModelOnSingleDataReady->funCallback);
						//! �˴���û���ر����Դ�Ĳ���,Ϊ���ڽ��濴��һ��һ��FireDataReadyEvent()��Ч��,ͣ��50msec�ٲ�����һ��.
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
	::EnterCriticalSection(&m_cs);
	m_dataRangesWaitingForExecute.push_back(r);
	::LeaveCriticalSection(&m_cs);
	return true;
}

bool XmlDataLoader::getRange(XmlDataLoader::range &r)
{
	::EnterCriticalSection(&m_cs);
	if (m_dataRangesWaitingForExecute.empty()==false)
	{
		r = m_dataRangesWaitingForExecute.front();
		m_dataRangesWaitingForExecute.erase(m_dataRangesWaitingForExecute.begin());
	}
	::LeaveCriticalSection(&m_cs);
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