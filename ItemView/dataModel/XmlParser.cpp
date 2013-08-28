#include "XmlParser.h"
#include "PostMessageToUIThread.h"
#include "xl_lib/text/transcode.h"

XmlParser::XmlParser()
{
}

XmlParser::~XmlParser()
{
	while(!m_playlist->empty())
	{
		m_playlist->pop_back();
	}
	delete m_playlist;
}

bool XmlParser::LoadPlaylist(std::vector<StrSongInfo> & playlist, const char* fileName)
{
	std::wstring wplaylistName;
	xl::text::transcode::UTF8_to_Unicode(fileName, strlen(fileName), wplaylistName);
	wchar_t *playlistName = new wchar_t[strlen(fileName)];
	wcscpy(playlistName, wplaylistName.c_str());

	assert(playlistName);
	XLFS_FILE_HANDLE hFile = NULL;
	m_playlist = &playlist;

	if (XLFS_OpenFile(playlistName, L"rb", &hFile, NULL) != XLFS_RESULT_SUCCESS)
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
		m_playlist->push_back(*info);
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