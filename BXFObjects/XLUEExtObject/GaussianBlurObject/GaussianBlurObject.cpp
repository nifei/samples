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
, m_type(DirecheIIRSSEIntrinsics)
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
		// FIR ÐÍÂË²¨
		if (m_type == TwoDimention)
		{
			TwoDimentionRender(hClipBitmap, m_sigma, m_radius);
		}
		else if (m_type == OneDimention)
		{
			OneDimentionRenderSSE(hClipBitmap, m_sigma, m_radius);
		}
		// IIR ÐÍÂË²¨
		else if (m_type == DirecheIIR)
		{
			DericheIIRRender(hClipBitmap, m_sigma);
		}
		else if (m_type == DirecheIIRSSE)
		{
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
