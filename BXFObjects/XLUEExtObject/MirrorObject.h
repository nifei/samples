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
*   FileName    :   MirrorObject
*   Author      :   ������
*   Create      :   2013-5-26 11:35
*   LastChange  :   2013-5-26 11:35
*   History     :	
*
*   Description :   ������󣬿��Զ�һ���������һ��������Ⱦ������о�����
*
********************************************************************/ 
#ifndef __MIRROROBJECT_H__
#define __MIRROROBJECT_H__


class MirrorObject
	: public ExtLayoutObjMethodsImpl
{
public:

	enum MirrorType
	{
		MirrorType_none = 0,
		MirrorType_hor = 1,
		MirrorType_ver = 2,
		MirrorType_both = 3,
	};

public:

	MirrorObject(XLUE_LAYOUTOBJ_HANDLE hObj);
	virtual ~MirrorObject(void);

	void SetMirrorType(MirrorType type);
	MirrorType GetMirrorType() const;

public:

	// ExtLayoutObjMethodsImpl virtual funtions

	// �ö����Ǽ������еģ�������Ҫ���LayerObject+clipsensʹ��
	virtual void OnPaint(XL_BITMAP_HANDLE hBitmapDest, const RECT* lpDestClipRect, const RECT* lpSrcClipRect, unsigned char alpha);

private:

	void HorizontalMirror(XL_BITMAP_HANDLE hBitmap) const;
	void VerticalMirror(XL_BITMAP_HANDLE hBitmap) const;
	void BothMirror(XL_BITMAP_HANDLE hBitmap) const;

private:

	MirrorType m_mirrorType;
};

#endif // __MIRROROBJECT_H__