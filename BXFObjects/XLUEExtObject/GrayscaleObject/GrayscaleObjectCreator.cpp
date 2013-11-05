/********************************************************************
/* Copyright (c) 2013 The BOLT UIEngine. All rights reserved.
/* Use of this source code is governed by a BOLT license that can be
/* found in the LICENSE file.
********************************************************************/ 
#include "stdafx.h"
#include "./GrayscaleObjectCreator.h"

GrayscaleObjectCreator::GrayscaleObjectCreator(void)
{
}

GrayscaleObjectCreator::~GrayscaleObjectCreator(void)
{
}

GrayscaleObject* GrayscaleObjectCreator::CreateObj( const char* lpObjClass, XLUE_LAYOUTOBJ_HANDLE hObj )
{
	assert(hObj);
	assert(lpObjClass);

	return new GrayscaleObject(hObj);
}

void GrayscaleObjectCreator::DestroyObj( GrayscaleObject* lpObj )
{
	assert(lpObj);

	delete lpObj;
}