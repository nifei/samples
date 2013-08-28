#include "XmlDataModel.h"
#include "string.h"
#include "XmlParser.h"

struct CallbackOnDataReady{
	funcDataReadyCallback pfnCallback;
	DWORD dwUserData1;
	DWORD dwUserData2;
};

XmlDataModel::XmlDataModel(int argc, const char *argv[])
:m_callbackOnDataReady(NULL), m_loader(NULL)
{
	if (argc && argv)
	{
		m_loader = new XmlDataLoader();
		m_loader->LoadPlaylist(m_playlist, argv[0]);
		m_loader->start();
	}
}

XmlDataModel::~XmlDataModel()
{
	if (m_loader)
	{
		delete m_loader;
		m_loader = 0;
	}
	if (m_callbackOnDataReady)
		delete m_callbackOnDataReady;
}

int XmlDataModel::GetCount()const
{return m_playlist.size();}

int XmlDataModel::GetColumnCount()const
{return MAX_COL-1;}

bool XmlDataModel::GetDataBatch(int from, int to, void **dataBatch, char** types)
{
	bool ret = false;
	if (types)
	{
		types[0] = "bitmap";
		types[1] = "char";
		types[2] = "char";
		if (from < 1) from = 1;
		if (to > (int)m_playlist.size()) to = m_playlist.size();
		if (from <= to)
		{
			for ( int row = from; row <= to; row++)
			{
				char *name = const_cast<char*>(m_playlist.at(row - 1).name.c_str());
				char* source = const_cast<char*>(m_playlist.at(row - 1).source.c_str());
				dataBatch[3*(row-from)] = new XL_BITMAP_HANDLE(m_playlist.at(row - 1).hBitmap);
				dataBatch[3*(row-from)+1] = name;
				dataBatch[3*(row-from)+2] = source;
			}
			ret = true;
		}
	}
	return ret;
}

char* XmlDataModel::GetItemAtIndex(int irow,int icolumn, void **itemData)
{
	size_t row(irow);
	size_t column(icolumn);
	char *dataType = "nil";

	if (row >= 1 && row <= m_playlist.size())
	{
		if (column == COVER_COL)
		{
			if(m_playlist.at(row-1).hBitmap == NULL)
			{
				m_playlist.at(row-1).hBitmap = m_loader->LoadImage(m_playlist.at(row-1).cover.c_str());
			}
			*itemData = m_playlist.at(row-1).hBitmap;
			dataType = "bitmap";
		}
		else if (column == NAME_COL)
		{
			std::string name = m_playlist.at(row-1).name;
			*itemData = new char[name.length()+1];
			strcpy((char*)(*itemData), name.c_str());
			dataType = "char";
		}
		else if (column == SOURCE_COL)
		{
			std::string source = m_playlist.at(row-1).source;
			*itemData = new char[source.length()+1];
			strcpy((char*)(*itemData), source.c_str());
			dataType = "char";
		}
	}
	return dataType;
}

void XmlDataModel::PrepareData(int from, int to)
{
	//准备的数据在m_playlist中的索引是from-1~to-1
	if (from <= 0) from = 1;
	if (to > m_playlist.size()) to = m_playlist.size();
	std::vector<StrSongInfo> *playlist = new std::vector<StrSongInfo>(to-from+1);
	std::vector<StrSongInfo>::const_iterator it = m_playlist.begin();
	std::vector<StrSongInfo>::const_iterator itBegin = it + from-1;
	std::vector<StrSongInfo>::const_iterator itEnd = it + to;//最后一个不包括在内
	playlist->assign(itBegin, itEnd);
	m_loader->PrepareData(from, playlist);
}

void XmlDataModel::ReleaseData(int from, int to)
{
	//准备的数据在m_playlist中的索引是from-1~to-1
	if (from <= 0) from = 1;
	if (to > m_playlist.size()) to = m_playlist.size();
	if (from <= to)
	{
		std::vector<StrSongInfo> *playlist = new std::vector<StrSongInfo>(to-from+1);
		std::vector<StrSongInfo>::const_iterator it = m_playlist.begin();
		std::vector<StrSongInfo>::const_iterator itBegin = it + from-1;
		std::vector<StrSongInfo>::const_iterator itEnd = it + to;//最后一个不包括在内
		playlist->assign(itBegin, itEnd);
		m_loader->ReleaseData(from, playlist);
	}
}

void XmlDataModel::FireDataReadyEvent(int row, int column)
{
	if (m_callbackOnDataReady)
	{
		m_callbackOnDataReady->pfnCallback(m_callbackOnDataReady->dwUserData1, m_callbackOnDataReady->dwUserData2, row, column);
	}
}

void XmlDataModel::FireDataReadyEvent(int from, std::vector<StrSongInfo> playlist)
{
	for (int i = 0; i < playlist.size(); i++)
	{
		m_playlist[from+i-1] = playlist.at(i);
	}
	FireDataReadyEvent(from, from+playlist.size()-1);
}

void XmlDataModel::FireDataReadyEvent(int row, StrSongInfo* song)
{
	m_playlist[row-1] = *song;
	for (int col = 1; col <= GetColumnCount(); col++)
	{
		FireDataReadyEvent(row, col);
	}
}

void XmlDataModel::SetSingleDataReadyListener(DWORD dwUserData1, DWORD dwUserData2, funcDataReadyCallback pfnCallback)
{
	if (m_callbackOnDataReady != 0)
		delete m_callbackOnDataReady;
	m_callbackOnDataReady = new CallbackOnDataReady();
	m_callbackOnDataReady->dwUserData1 = dwUserData1;
	m_callbackOnDataReady->dwUserData2 = dwUserData2;
	m_callbackOnDataReady->pfnCallback = pfnCallback;

	m_loader->SetSingleDataReadyListener(XmlDataModel::UIThreadCallbackOnSingleData, (void*)this);
}

void XmlDataModel::SetDataBatchReadyListener(DWORD dwUserData1, DWORD dwUserData2, funcDataReadyCallback pfnCallback)
{
	if (m_callbackOnDataReady != 0)
		delete m_callbackOnDataReady;
	m_callbackOnDataReady = new CallbackOnDataReady();
	m_callbackOnDataReady->dwUserData1 = dwUserData1;
	m_callbackOnDataReady->dwUserData2 = dwUserData2;
	m_callbackOnDataReady->pfnCallback = pfnCallback;

	m_loader->SetDataBatchReadyListener(XmlDataModel::UIThreadCallbackOnDataBatch, (void*)this);
}

void XmlDataModel::UIThreadCallbackOnDataBatch(void *userData)
{
	if (PostDataBatchMessageToUIThreadUserData *myData = (PostDataBatchMessageToUIThreadUserData*)userData)
	{
		if (XmlDataModel *dataModel = (XmlDataModel*)myData->ptrCaller)
		{
			dataModel->FireDataReadyEvent(myData->from, myData->list);
		}
	}
}

void XmlDataModel::UIThreadCallbackOnSingleData(void *userData)
{
	if (PostSingleDataMessageToUIThreadUserData *myData = (PostSingleDataMessageToUIThreadUserData*)userData)
	{
		if (XmlDataModel *dataModel = (XmlDataModel*)myData->ptrCaller)
		{
			dataModel->FireDataReadyEvent(myData->row, myData->songInfo);
		}
	}
}