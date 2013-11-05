/********************************************************************
/* Copyright (c) 2013 The BOLT UIEngine. All rights reserved.
/* Use of this source code is governed by a BOLT license that can be
/* found in the LICENSE file.
********************************************************************/ 
#ifndef __GRAYSCALEOBJECTPARSER_H__
#define __GRAYSCALEOBJECTPARSER_H__

#include "./GrayscaleObject.h"

class GrayscaleObjectParser
	: public ExtObjParserImpl<GrayscaleObject>
{
public:
	GrayscaleObjectParser(void);
	virtual ~GrayscaleObjectParser(void);

private:

	// ExtObjParserImpl
	virtual bool ParserAttribute(GrayscaleObject* lpObj, const char* key, const char* value);
};

#endif // __GrayscaleObjectPARSER_H__