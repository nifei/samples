#include "XmlDataModel.h"
#include "string.h"
#include "XmlParser.h"

using namespace std;

const char* MY_SOURCE=("src");
const char* MY_COVER=("cover");
const char* MY_NAME=("name");

const unsigned int MAX_LEN=255;

XmlDataModel::XmlDataModel(int argc, const char *argv[])
:m_callbackOnDataReady(NULL), m_parser(NULL)
{
	if (argc && argv)
	{
		m_parser = new XmlParser(const_cast<char*>(argv[0]));
		m_parser->start();
	}
}

void XmlDataModel::initialize()
{
}

XmlDataModel::~XmlDataModel()
{
	if (m_parser)
		delete m_parser;
	if (m_callbackOnDataReady)
		delete m_callbackOnDataReady;
}

int XmlDataModel::GetCount(const char* view)const
{return m_parser->GetCount();}

int XmlDataModel::GetColumnCount(const char *view)const
{
	return m_parser->GetColumnCount();
}

bool XmlDataModel::GetDataBatch(int from, int to, const char* view, void **dataBatch, char** types)
{
	m_parser->GetDataBatch(from, to, dataBatch, types);
	return true;
}

char* XmlDataModel::GetItemAtIndex(int irow,int icolumn,const char*view, void **itemData)
{
	size_t row(irow);
	size_t column(icolumn);
	if (row > 0 && row <= m_parser->GetCount())
	{
		char *type = "";
		if(m_parser->GetDataAtIndex(row, column, itemData, &type))
			return type;
	}
	return "nil";
}

void XmlDataModel::PrepareData(int from, int to, const char *userdata)
{
	m_parser->PrepareData(from, to);
}

void XmlDataModel::ReleaseData(int from, int to, const char* userdata)
{
	m_parser->ReleaseData(from, to);
}

void XmlDataModel::FireDataReadyEvent(int row, int column)
{
	if (m_callbackOnDataReady)
	{
		m_callbackOnDataReady->pfnCallback(m_callbackOnDataReady->dwUserData1, m_callbackOnDataReady->dwUserData2, row, column);
	}
}

void XmlDataModel::AttachSingleDataReadyListener(DWORD dwUserData1, DWORD dwUserData2, funcDataReadyCallback pfnCallback)
{
	m_parser->AttachSingleDataReadyListener(dwUserData1, dwUserData2, pfnCallback);
}

void XmlDataModel::AttachDataBatchReadyListener(DWORD dwUserData1, DWORD dwUserData2, funcDataReadyCallback pfnCallback)
{
	m_parser->AttachDataBatchReadyListener(dwUserData1, dwUserData2, pfnCallback);
}
