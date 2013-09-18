/********************************************************************
/* Copyright (c) 2013 The BOLT UIEngine. All rights reserved.
/* Use of this source code is governed by a BOLT license that can be
/* found in the LICENSE file.
********************************************************************/ 
#include "stdafx.h"
#include "./MirrorObjectParser.h"
#include "./LuaMirrorObject.h"

MirrorObjectParser::MirrorObjectParser(void)
{
}

MirrorObjectParser::~MirrorObjectParser(void)
{
}

bool MirrorObjectParser::ParserAttribute( MirrorObject* lpObj, const char* key, const char* value )
{
	assert(lpObj);
	assert(key);
	assert(value);

	bool ret = true;
	if (strcmp(key, "mirrortype") == 0)
	{
		MirrorObject::MirrorType type = LuaMirrorObject::GetMirrorTypeFromString(value);
		lpObj->SetMirrorType(type);
	}
	else
	{
		assert(false);
		ret = false;
	}

	return ret;
}