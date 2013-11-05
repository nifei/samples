/********************************************************************
/* Copyright (c) 2013 The BOLT UIEngine. All rights reserved.
/* Use of this source code is governed by a BOLT license that can be
/* found in the LICENSE file.
********************************************************************/ 
#include "stdafx.h"
#include "./GrayscaleObjectRegister.h"
#include "BXF/ExtObjRegisterHelper.h"
#include "./GrayscaleObject.h"
#include "./GrayscaleObjectCreator.h"
#include "./GrayscaleObjectParser.h"
#include "./LuaGrayscaleObject.h"

BOOL GrayscaleObjectRegister::RegisterGrayscaleObject()
{
	unsigned long attribute = ExtObjAttribute_clipsens;

	return ExtObjRegisterHelper<ExtObjType_renderableObj, GrayscaleObject, GrayscaleObjectCreator, 
		GrayscaleObjectParser, LuaGrayscaleObject>::Register(EXTCLASSNAME_GRAYSCALEOBJECT, attribute);
}