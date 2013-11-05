/********************************************************************
/* Copyright (c) 2013 The BOLT UIEngine. All rights reserved.
/* Use of this source code is governed by a BOLT license that can be
/* found in the LICENSE file.
********************************************************************/ 
#include "stdafx.h"
#include "./GrayscaleObjectParser.h"
#include "./LuaGrayscaleObject.h"

GrayscaleObjectParser::GrayscaleObjectParser(void)
{
}

GrayscaleObjectParser::~GrayscaleObjectParser(void)
{
}

bool GrayscaleObjectParser::ParserAttribute( GrayscaleObject* lpObj, const char* key, const char* value )
{
	assert(lpObj);
	assert(key);
	assert(value);

	bool ret = true;
	
	{
		assert(false);
		ret = false;
	}

	return ret;
}