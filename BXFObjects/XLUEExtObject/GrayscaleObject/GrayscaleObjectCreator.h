/********************************************************************
/* Copyright (c) 2013 The BOLT UIEngine. All rights reserved.
/* Use of this source code is governed by a BOLT license that can be
/* found in the LICENSE file.
********************************************************************/ 
#ifndef __GRAYSCALEOBJECTCREATOR_H__
#define __GRAYSCALEOBJECTCREATOR_H__

#include "./GrayscaleObject.h"

class GrayscaleObjectCreator
	: public ExtObjCreatorImpl<GrayscaleObject>
{
public:
	GrayscaleObjectCreator(void);
	virtual ~GrayscaleObjectCreator(void);

private:

	virtual GrayscaleObject* CreateObj(const char* lpObjClass, XLUE_LAYOUTOBJ_HANDLE hObj);
	virtual void DestroyObj(GrayscaleObject* lpObj);
};

#endif // __GrayscaleObjectCREATOR_H__