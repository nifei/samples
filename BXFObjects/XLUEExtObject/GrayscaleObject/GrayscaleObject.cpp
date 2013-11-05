/********************************************************************
/* Copyright (c) 2013 The BOLT UIEngine. All rights reserved.
/* Use of this source code is governed by a BOLT license that can be
/* found in the LICENSE file.
********************************************************************/ 
#include "stdafx.h"

#include "./GrayscaleObject.h"

#include "./GrayscaleImpl.h"

#include <iostream>
#include <time.h>

GrayscaleObject::GrayscaleObject( XLUE_LAYOUTOBJ_HANDLE hObj )
:ExtLayoutObjMethodsImpl(hObj)
{
}

GrayscaleObject::~GrayscaleObject(void)
{
}

void GrayscaleObject::OnPaint( XL_BITMAP_HANDLE hBitmapDest, const RECT* lpDestClipRect, const RECT* lpSrcClipRect, unsigned char alpha )
{       
	clock_t time1 = clock();

	assert(lpSrcClipRect);
	const RECT* pos = GetPos();

	XL_BITMAP_HANDLE hClipBitmap = XL_ClipSubBindBitmap(hBitmapDest, lpDestClipRect);
	assert(hClipBitmap);

	for (int i = 0; i < 1; i++)
	AveragingGrayscale(hClipBitmap);

	XL_ReleaseBitmap(hClipBitmap);
	clock_t time2=clock();
	float diff = (((float)time2 - (float)time1) / 1000000.0F ) * 1000; 
}
