#include "XmlParser.h"
#include "PostMessageToUIThread.h"
#include "xl_lib/text/transcode.h"

struct StrSongInfo
{
	std::string cover;
	std::string name;
	std::string source;
	XL_BITMAP_HANDLE hBitmap;

	StrSongInfo()
		:cover(std::string()),
		name(std::string()),
		source(std::string()),
		hBitmap(0){}
	StrSongInfo(std::string _cover, std::string _name, std::string _source)
		:cover(_cover),
		name(_name),
		source(_source){}
};

struct XmlParser::range
{
	int from;
	int to;
	int type;
	enum{Prepare, Release, Invalid};
	XmlParser::range(int _from, int _to, int _type)
		:from(_from), to(_to), type(_type){}
	XmlParser::range()
		:type(Invalid){}
};

XL_BITMAP_HANDLE XmlParser::LoadPng( const wchar_t* lpFile )
{
	assert(lpFile);
	XL_BITMAP_HANDLE hBitmap = XL_LoadBitmapFromFile(lpFile, XLGRAPHIC_CT_ARGB32);
	assert(hBitmap);
	return hBitmap;
}

XL_BITMAP_HANDLE XmlParser::loadImage(const char* lpFile)
{
	std::wstring wlpFile;
	xl::text::transcode::UTF8_to_Unicode(lpFile, strlen(lpFile), wlpFile);
	return loadImage(wlpFile.c_str());
}

XL_BITMAP_HANDLE XmlParser::loadImage( const wchar_t* lpFile )
{
	const wchar_t* lpExt = ::PathFindExtension(lpFile);
	if (lpExt && ::lstrcmpi(lpExt, L".png") == 0)
	{
		return LoadPng(lpFile); 
	}
	return NULL;
}

XmlParser::XmlParser(wchar_t *fileName)
:m_playlistName(fileName), 
m_callbackOnSingleDataReady(0),
m_callbackOnDataBatchReady(0), 
m_mutexOnPlaylist(0), 
m_mutexOnRangeList(0)
{
	init();
}

XmlParser::XmlParser(char* fileName)
:m_callbackOnSingleDataReady(0),
m_callbackOnDataBatchReady(0), 
m_mutexOnPlaylist(0), 
m_mutexOnRangeList(0)
{
	std::wstring wplaylistName;
	xl::text::transcode::UTF8_to_Unicode(fileName, strlen(fileName), wplaylistName);
	m_playlistName = new wchar_t[strlen(fileName)];
	wcscpy(m_playlistName, wplaylistName.c_str());
	init();
}

void XmlParser::init()
{
	loadPlaylist();
	m_mutexOnPlaylist = new xl::win32::multithread::mutex(false, L"PlaylistMutex");
	m_mutexOnRangeList= new xl::win32::multithread::mutex(false, L"RangeListMutex");
	InitUIThread();
}

XmlParser::~XmlParser()
{
	while(!m_playlist->empty())
	{
		m_playlist->pop_back();
	}
	delete m_playlist;
	if (m_mutexOnPlaylist)
	{
		m_mutexOnPlaylist->unlock();
		delete m_mutexOnPlaylist;
	}
	if (m_mutexOnRangeList)
	{
		m_mutexOnRangeList->unlock();
		delete m_mutexOnRangeList;
	}
	if (m_playlistName != NULL)
		delete []m_playlistName;
	UnInitUIThread();
}

/*!
这是一个简单的示例, 在ItemView调用DataModel的PrepareData时, 应该准备加载数据. 
在实际应用中, DataModel在PrepareData是才真正加载数据, 而不是像Demo中这样直接从list中读取.
*/
bool XmlParser::PrepareData(int from, int to)
{
	assert(from <= to);
	if (from == 0 || from == -1) from = 1;
	if (to > (int)m_playlist->size()) to = m_playlist->size();
	if (to <= 0) to = 1;
	m_mutexOnRangeList->lock();
	range r(from, to, range::Prepare);
	m_dataRangesWaitingForExecute.push_back(r);
	m_mutexOnRangeList->unlock();
	return true;
}

/*!
注意: PrepareData和ReleaseData要在同一线程中操作m_playlist; 否则会有问题
*/
bool XmlParser::ReleaseData(int from, int to)
{
	if (from == 0)
		from = 1;
	if (from == -1)
		from = 1;
	if (to == -1)
		to = 0;
	if (to > (int)m_playlist->size())
		to = m_playlist->size();
	m_mutexOnRangeList->lock();
	range r(from, to, range::Release);
	m_dataRangesWaitingForExecute.push_back(r);
	m_mutexOnRangeList->unlock();
	return true;
}

void XmlParser::RunInMainThread()
{
}

struct PostUIThreadUserData
{
	enum{SingleDataReady, DataBatchReady};
	int type;
	XmlParser *parser;
	int arg1;
	int arg2;
	PostUIThreadUserData(XmlParser *_parser, int _arg1, int _arg2, int _type)
		:parser(_parser), arg1(_arg1), arg2(_arg2), type(_type){}
};

xl::uint32  XmlParser::thread_proc()
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
			for ( int row = r.from; row <= r.to; row++)
			{
					m_mutexOnPlaylist->lock();
					m_playlist->at(row-1)->hBitmap = loadImage(m_playlist->at(row-1)->cover.c_str());
					m_mutexOnPlaylist->unlock();
					if(PostMessageToUIThread && m_callbackOnSingleDataReady)
					{
						PostUIThreadUserData *u1 = new PostUIThreadUserData(this, row, 1, PostUIThreadUserData::SingleDataReady);
						PostUIThreadUserData *u2 = new PostUIThreadUserData(this, row, 2, PostUIThreadUserData::SingleDataReady);
						PostUIThreadUserData *u3 = new PostUIThreadUserData(this, row, 3, PostUIThreadUserData::SingleDataReady);
						PostMessageToUIThread((void*)u1, XmlParser::UIThreadCallback);
						PostMessageToUIThread((void*)u2, XmlParser::UIThreadCallback);
						PostMessageToUIThread((void*)u3, XmlParser::UIThreadCallback);
						//! 此处并没有特别耗资源的操作,为了在界面看到一行一行FireDataReadyEvent()的效果,停顿50msec再操作下一行.
						Sleep(50);
					}
			}
			if (PostMessageToUIThread && m_callbackOnDataBatchReady)
			{
				PostUIThreadUserData *u = new PostUIThreadUserData(this, r.from, r.to, PostUIThreadUserData::DataBatchReady);
				PostMessageToUIThread((void*)u, XmlParser::UIThreadCallback);
			}
		} else if (r.type == range::Release)
		{
			for (int row = r.from; row <= r.to; row++)
			{
				m_mutexOnPlaylist->lock();
				if(m_playlist->at(row-1)->hBitmap != NULL)
				{
					///! 创建资源的对象释放资源
					XL_ReleaseBitmap(m_playlist->at(row-1)->hBitmap);
					m_playlist->at(row-1)->hBitmap = NULL;
				}
				m_mutexOnPlaylist->unlock();
			}
		}
	}// while true
	return 1;
}

void XmlParser::SetSingleDataReadyListener(DWORD dwUserData1, DWORD dwUserData2, funcDataReadyCallback pfnCallback)
{
	if (m_callbackOnSingleDataReady)
		delete m_callbackOnSingleDataReady;
	m_callbackOnSingleDataReady = new CallbackOnDataReady();
	m_callbackOnSingleDataReady->dwUserData1 = dwUserData1;
	m_callbackOnSingleDataReady->dwUserData2 = dwUserData2;
	m_callbackOnSingleDataReady->pfnCallback = pfnCallback;
}

void XmlParser::SetDataBatchReadyListener(DWORD dwUserData1, DWORD dwUserData2, funcDataReadyCallback pfnCallback)
{
	if (m_callbackOnDataBatchReady)
		delete m_callbackOnDataBatchReady;
	m_callbackOnDataBatchReady = new CallbackOnDataReady();
	m_callbackOnDataBatchReady->dwUserData1 = dwUserData1;
	m_callbackOnDataBatchReady->dwUserData2 = dwUserData2;
	m_callbackOnDataBatchReady->pfnCallback = pfnCallback;
}

void XmlParser::UIThreadCallback(void* _userData)
{
	if (PostUIThreadUserData *userData = (PostUIThreadUserData*)_userData)
	{
		if (userData->type == PostUIThreadUserData::SingleDataReady)
		{
			XmlParser *parser = userData->parser;
			int row = userData->arg1;
			int column = userData->arg2;
			parser->FireDataReadyEvent(row, column);
		} else if (userData->type == PostUIThreadUserData::DataBatchReady)
		{
			XmlParser *parser = userData->parser;
			int from = userData->arg1;
			int to = userData->arg2;
			parser->FireDataBatchReadyEvent(from, to);
		}
		delete _userData;
	}
}

void XmlParser::FireDataReadyEvent(int row, int column)
{
	if (m_callbackOnSingleDataReady)
	{
		m_callbackOnSingleDataReady->pfnCallback(m_callbackOnSingleDataReady->dwUserData1, m_callbackOnSingleDataReady->dwUserData2, row, column);
	}
}

void XmlParser::FireDataBatchReadyEvent(int from, int to)
{
	if (m_callbackOnDataBatchReady)
	{
		m_callbackOnDataBatchReady->pfnCallback(m_callbackOnDataBatchReady->dwUserData1, m_callbackOnDataBatchReady->dwUserData2, from, to);
	}
}

bool XmlParser::GetDataBatch(int from, int to, void **dataBatch,  char **types)
{
	bool ret = false;
	if (m_mutexOnPlaylist->try_lock(1000))
	{
		if (types)
		{
			types[0] = "bitmap";
			types[1] = "char";
			types[2] = "char";
			if (from < 1) from = 1;
			if (to > (int)m_playlist->size()) to = m_playlist->size();
			if (from <= to)
			{
				for ( int row = from; row <= to; row++)
				{
					char *name = const_cast<char*>(m_playlist->at(row - 1)->name.c_str());
					char* source = const_cast<char*>(m_playlist->at(row - 1)->source.c_str());
					dataBatch[3*(row-from)] = new XL_BITMAP_HANDLE(m_playlist->at(row - 1)->hBitmap);
					dataBatch[3*(row-from)+1] = name;
					dataBatch[3*(row-from)+2] = source;
				}
				ret = true;
			}
		}
		m_mutexOnPlaylist->unlock();
	}
	return ret;
}

bool XmlParser::GetDataAtIndex(int row, int column, void **itemData, char** dataType)
{
	bool ret = false;
	if (m_mutexOnPlaylist->try_lock(1000))
	{
		if (row >= 1 && row <= m_playlist->size())
		{
			if (column == COVER_COL)
			{
				if(m_playlist->at(row-1)->hBitmap == NULL)
				{
					m_playlist->at(row-1)->hBitmap = loadImage(m_playlist->at(row-1)->cover.c_str());
				}
				*itemData = m_playlist->at(row-1)->hBitmap;
				*dataType = "bitmap";
				ret = true;
			}
			else if (column == NAME_COL)
			{
				std::string name = m_playlist->at(row-1)->name;
				*itemData = new char[name.length()+1];
				strcpy((char*)(*itemData), name.c_str());
				//sprintf((char*)(*itemData), "%i", row);
				*dataType = "char";
				ret = true;
			}
			else if (column == SOURCE_COL)
			{
				std::string source = m_playlist->at(row-1)->source;
				*itemData = new char[source.length()+1];
				strcpy((char*)(*itemData), source.c_str());
				*dataType = "char";
				ret = true;
			}
		}
		m_mutexOnPlaylist->unlock();
	}
	return ret;
}

bool XmlParser::loadPlaylist()
{
	assert(m_playlistName);
	XLFS_FILE_HANDLE hFile = NULL;
	m_playlist = new std::vector<StrSongInfo*>();

	if (XLFS_OpenFile(m_playlistName, L"rb", &hFile, NULL) != XLFS_RESULT_SUCCESS)
		return false;

	char buffer[1024 * 16];
	__int64 nReadLength = 0;
	__int64 nFilePosition = 0;
	bool fSuccess = true;
	if (!create())
	{
		XLFS_CloseFile(hFile);
		return false;
	}

	attrNamesOfColumn[MEDIA_TAG] = ("media");
	attrNamesOfColumn[COVER_COL] = ("cover");
	attrNamesOfColumn[NAME_COL] = ("name");
	attrNamesOfColumn[SOURCE_COL] = ("src");

	while(!XLFS_IsEOF(hFile) && fSuccess)
	{
		nReadLength = XLFS_ReadFile(hFile, (unsigned char*)buffer, (int)sizeof(char) *1024 * 16);
		fSuccess = this->parse(buffer, (int)nReadLength, 0);
	}

	XLFS_CloseFile(hFile);
	return true;
}

/*!
XML库方法
*/
void XmlParser::OnStartElement(const XML_Char *pszName, const XML_Char **papszAttrs)
{
	if (strcmp(pszName, this->attrNamesOfColumn[MEDIA_TAG]) == 0)
	{
		StrSongInfo *info = new StrSongInfo();
		int attrInd = 0;
		const XML_Char * attr = papszAttrs[attrInd];
		while(attr)
		{
			if (strcmp(attr, attrNamesOfColumn[COVER_COL]) == 0)
			{
				attrInd++;
				attr = papszAttrs[attrInd];
				info->cover = attr;
			}
			if (strcmp(attr, attrNamesOfColumn[NAME_COL]) == 0)
			{
				attrInd++;
				attr = papszAttrs[attrInd];
				info->name = attr;
			}
			if (strcmp(attr, attrNamesOfColumn[SOURCE_COL]) == 0)
			{
				attrInd++;
				attr = papszAttrs[attrInd];
				info->source = attr;
			}
			attrInd++;
			attr = papszAttrs[attrInd];
		}
		m_playlist->push_back(info);
	}
}

/*!
XML库方法
*/
void XmlParser::OnEndElement(const XML_Char * c)
{
	return;
}

/*!
XML库方法
*/
void XmlParser::OnPostCreate()
{
	enable_element_handler();
}