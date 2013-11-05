/********************************************************************
/* Copyright (c) 2013 The BOLT UIEngine. All rights reserved.
/* Use of this source code is governed by a BOLT license that can be
/* found in the LICENSE file.
********************************************************************/ 
#ifndef __LUAGRAYSCALEOBJECT_H__
#define __LUAGRAYSCALEOBJECT_H__

#include "./GrayscaleObject.h"

class LuaGrayscaleObject
	: public ExtObjLuaHostImplEx<LuaGrayscaleObject, GrayscaleObject>
{
public:
	LuaGrayscaleObject(void);
	virtual ~LuaGrayscaleObject(void);

public:

private:

public:
	static const XLLRTGlobalAPI s_szLuaMemberFuncs[];
};

#endif // __LUAGrayscaleObject_H__