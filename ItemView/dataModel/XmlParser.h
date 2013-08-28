#ifndef _XML_PARSER_H_
#define _XML_PARSER_H_
#pragma once 

#include <xl_lib/xml/expat_parser.h>
#include <vector>
#include "stdafx.h"
#include "xl_lib/multithread/thread.h"
#include "xl_lib/multithread/mutex.h"

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
	StrSongInfo(const StrSongInfo &other)
		:cover(other.cover), 
		name(other.name),
		source(other.source),
		hBitmap(other.hBitmap){}
	const StrSongInfo operator= (const StrSongInfo & rhs)
	{
		cover = rhs.cover;
		name = rhs.name;
		source = rhs.source;
		hBitmap = rhs.hBitmap;
		return *this;
	}
};

class XmlParser : public xl::xml::expat_parser<XmlParser>
{
public:
	XmlParser();
	~XmlParser();
	bool LoadPlaylist(std::vector<StrSongInfo> & playlist, const char* fileName);

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

private:
	char * attrNamesOfColumn[MAX_COL];
	std::vector<StrSongInfo> *m_playlist;
};

#endif