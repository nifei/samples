/********************************************************************
/* Copyright (c) 2013 The BOLT UIEngine. All rights reserved.
/* Use of this source code is governed by a BOLT license that can be
/* found in the LICENSE file.
********************************************************************/ 
/********************************************************************
*
* =-----------------------------------------------------------------=
* =                                                                 =
* =             Copyright (c) Xunlei, Ltd. 2004-2013              =
* =                                                                 =
* =-----------------------------------------------------------------=
* 
*   FileName    :   MirrorObjectParser
*   Author      :   ������
*   Create      :   2013-5-26 13:30
*   LastChange  :   2013-5-26 13:30
*   History     :	
*
*   Description :   MirrorObject��xml���������
*
********************************************************************/ 
#ifndef __MIRROROBJECTPARSER_H__
#define __MIRROROBJECTPARSER_H__

#include "./MirrorObject.h"

class MirrorObjectParser
	: public ExtObjParserImpl<MirrorObject>
{
public:
	MirrorObjectParser(void);
	virtual ~MirrorObjectParser(void);

private:

	// ExtObjParserImpl
	virtual bool ParserAttribute(MirrorObject* lpObj, const char* key, const char* value);
};

#endif // __MIRROROBJECTPARSER_H__