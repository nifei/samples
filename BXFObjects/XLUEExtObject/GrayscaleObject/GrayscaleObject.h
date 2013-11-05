/********************************************************************
/* Copyright (c) 2013 The BOLT UIEngine. All rights reserved.
/* Use of this source code is governed by a BOLT license that can be
/* found in the LICENSE file.
********************************************************************/ 
#ifndef __GRAYSCALEOBJECT_H__
#define __GRAYSCALEOBJECT_H__

class GrayscaleObject
	: public ExtLayoutObjMethodsImpl
{
public:
	GrayscaleObject(XLUE_LAYOUTOBJ_HANDLE hObj);
	virtual ~GrayscaleObject(void);

public:

	// ExtLayoutObjMethodsImpl virtual funtions

	// 该对象是剪裁敏感的，所以需要配合LayerObject+clipsens使用
	virtual void OnPaint(XL_BITMAP_HANDLE hBitmapDest, const RECT* lpDestClipRect, const RECT* lpSrcClipRect, unsigned char alpha);
private:
};

#endif // __GrayscaleObject_H__