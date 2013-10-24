/********************************************************************
/* Copyright (c) 2013 The BOLT UIEngine. All rights reserved.
/* Use of this source code is governed by a BOLT license that can be
/* found in the LICENSE file.
********************************************************************/ 
#include "stdafx.h"
#include "./GaussianBlurObject.h"
#include "GaussianBlurDelegate.h"

#include <iostream>
#include <time.h>

GaussianBlurObject::GaussianBlurObject( XLUE_LAYOUTOBJ_HANDLE hObj )
:ExtLayoutObjMethodsImpl(hObj)
, m_sigma(0)
, m_radius(1)
, m_type(DirecheIIRSSE)
{
}

GaussianBlurObject::~GaussianBlurObject(void)
{
}

void GaussianBlurObject::SetSigma(double sigma)
{
	if (m_sigma == sigma)
	{
		return;
	}
	m_sigma = sigma;
	PushDirtyRect(NULL);
}

void GaussianBlurObject::OnPaint( XL_BITMAP_HANDLE hBitmapDest, const RECT* lpDestClipRect, const RECT* lpSrcClipRect, unsigned char /*alpha*/ )
{
	clock_t time1 = clock();

	assert(lpSrcClipRect);
	const RECT* pos = GetPos();

	XL_BITMAP_HANDLE hClipBitmap = XL_ClipSubBindBitmap(hBitmapDest, lpDestClipRect);
	assert(hClipBitmap);

	if (m_radius >= 0 && m_sigma >0)
	{
		// FIR 型滤波
		if (m_type == TwoDimention)
		{
			TwoDimentionRender(hClipBitmap, m_sigma, m_radius);
		}
		else if (m_type == OneDimentionMMX)
		{
			// 亚星: 循环时对边界若干像素做单独处理, 中间像素没必要每一个比较范围(有提升)
			// 司司: shift舍弃精度, 有很大提升, 半径>128时怎么处理还不知道-_-?!
			// 
			for (int i = 0; i < 10; i++)
			OneDimentionRenderMMX(hClipBitmap, m_sigma, m_radius);
		}
		else if (m_type == OneDimentionSSE)
		{
			OneDimentionRenderSSE(hClipBitmap, m_sigma, m_radius);
		}
		// IIR 型滤波
		else if (m_type == DirecheIIR)
		{
			DericheIIRRender(hClipBitmap, m_sigma);
		}
		else if (m_type == DirecheIIRSSE)
		{
			for (int i = 0; i < 1; i++)
			DericheIIRRenderSSE(hClipBitmap, m_sigma);
		}
		else if (m_type == DirecheIIRSSEIntrinsics)
		{
			DericheIIRRenderSSEIntrinsics(hClipBitmap, m_sigma);
		}
	}

	XL_ReleaseBitmap(hClipBitmap);
	clock_t time2=clock();
	float diff = (((float)time2 - (float)time1) / 1000000.0F ) * 1000; 
}
