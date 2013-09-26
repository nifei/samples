#include "XmlDataModel.h"
#include "string.h"
#include "XmlParser.h"

XmlDataModel::XmlDataModel(int argc, const char *argv[])
: m_loader(NULL)
, m_dataReadyListener(NULL)
{
	if (argc && argv)
	{
		InitUIThread();
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
		m_loader = NULL;
	}
	if (m_dataReadyListener)
	{
		delete m_dataReadyListener;
		m_dataReadyListener = NULL;
	}

	UnInitUIThread();
	//  no need to destroy m_playlist it'll be cleared then destroy as an object member of this XmlDataModel
}

int XmlDataModel::GetCount()
{
	return m_playlist.size();
}

int XmlDataModel::GetColumnCount()
{
	return MAX_COL-1;
}

bool XmlDataModel::GetDataBatch(int from, int to, void **dataBatch, char** types)
{
	bool ret = false;
	if (types)
	{
		types[0] = "bitmap";
		types[1] = "string";
		types[2] = "string";
		if (from < 1) from = 1;
		if (to > (int)m_playlist.size()) to = m_playlist.size();
		if (from <= to)
		{
			for ( int row = from; row <= to; row++)
			{
				dataBatch[3*(row-from)] = &(m_playlist.at(row - 1).hBitmap);
				dataBatch[3*(row-from)+1] = (void*)m_playlist.at(row - 1).name.c_str();
				dataBatch[3*(row-from)+2] = (void*)m_playlist.at(row - 1).source.c_str();
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
			*itemData = (void*)m_playlist.at(row-1).name.c_str();
			dataType = "string";
		}
		else if (column == SOURCE_COL)
		{
			*itemData = (void*)m_playlist.at(row-1).source.c_str();
			dataType = "string";
		}
	}
	return dataType;
}

void XmlDataModel::PrepareData(int from, int to)
{
	//准备的数据在m_playlist中的索引是from-1~to-1
	if (from <= 0) from = 1;
	if (to > m_playlist.size()) to = m_playlist.size();
	m_loader->PrepareData(from, to, m_playlist);
}

void XmlDataModel::ReleaseData(int from, int to)
{
	//准备的数据在m_playlist中的索引是from-1~to-1
	if (from <= 0) from = 1;
	if (to > m_playlist.size()) to = m_playlist.size();
	if (from <= to)
	{
		m_loader->ReleaseData(from, to, m_playlist);
		for(int i = from; i <= to; i++)
		{
			m_playlist[i-1].hBitmap = 0;
		}
	}
}

void XmlDataModel::FireDataReadyEvent(int row, int column)
{
	if (m_dataReadyListener)
	{
		m_dataReadyListener->onDataReady(row, column);
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

void XmlDataModel::FireDataReadyEvent(int row, const StrSongInfo& song)
{
	m_playlist[row-1] = song;
	for (int col = 1; col <= GetColumnCount(); col++)
	{
		FireDataReadyEvent(row, col);
	}
}

void XmlDataModel::SetSingleDataReadyListener(DataReadyListenerInterface *dataReadyListener)
{
	if (m_dataReadyListener)
	{
		delete m_dataReadyListener;
		m_dataReadyListener = NULL;
	} 
	else 
	{
		m_loader->SetSingleDataReadyListener(XmlDataModel::UIThreadCallbackOnSingleData, (void*)this);
	}
	m_dataReadyListener = dataReadyListener;
}

void XmlDataModel::SetDataBatchReadyListener(DataReadyListenerInterface *dataReadyListener)
{
	if (m_dataReadyListener)
	{
		delete m_dataReadyListener;
		m_dataReadyListener = NULL;
	}
	else
	{
		m_loader->SetDataBatchReadyListener(XmlDataModel::UIThreadCallbackOnDataBatch, (void*)this);
	}
	m_dataReadyListener = dataReadyListener;
}

void XmlDataModel::UIThreadCallbackOnDataBatch(void *userData)
{
	if (PostDataBatchMessageToUIThreadUserData *myData = (PostDataBatchMessageToUIThreadUserData*)userData)
	{
		if (XmlDataModel *dataModel = (XmlDataModel*)myData->ptrCaller)
		{
			dataModel->FireDataReadyEvent(myData->from, myData->list);
		}
		delete myData;
	}
}

void XmlDataModel::UIThreadCallbackOnSingleData(void *userData)
{
	if (PostSingleDataMessageToUIThreadUserData *myData = (PostSingleDataMessageToUIThreadUserData*)userData)
	{
		if (XmlDataModel *dataModel = (XmlDataModel*)myData->ptrCaller)
		{
			dataModel->FireDataReadyEvent(myData->row, myData->song);
		}
		delete myData;
	}
}