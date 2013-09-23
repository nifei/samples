/********************************************************************
/* Copyright (c) 2013 The BOLT UIEngine. All rights reserved.
/* Use of this source code is governed by a BOLT license that can be
/* found in the LICENSE file.
********************************************************************/ 
#include "stdafx.h"
#include "./GaussianBlurObject.h"

GaussianBlurObject::GaussianBlurObject( XLUE_LAYOUTOBJ_HANDLE hObj )
:ExtLayoutObjMethodsImpl(hObj)
{

}

GaussianBlurObject::~GaussianBlurObject(void)
{
}

void GaussianBlurObject::OnPaint( XL_BITMAP_HANDLE hBitmapDest, const RECT* lpDestClipRect, const RECT* lpSrcClipRect, unsigned char /*alpha*/ )
{
	assert(lpSrcClipRect);
	assert(RectHelper::EqualRect(lpSrcClipRect, GetPos()));

	XL_BITMAP_HANDLE hClipBitmap = XL_ClipSubBindBitmap(hBitmapDest, lpDestClipRect);
	assert(hClipBitmap);

	Simple(hClipBitmap);
	XL_ReleaseBitmap(hClipBitmap);
}

void GaussianBlurObject::Simple(XL_BITMAP_HANDLE hBitmap)const
{
	assert(hBitmap);
	XLBitmapInfo bmp;
	XL_GetBitmapInfo(hBitmap, &bmp);

	assert(bmp.ColorType == XLGRAPHIC_CT_ARGB32);
	//double *weight = new double[3*3];
	//for ( int i = -1; i <= 1; i++)
	//	for (int j = -1; j<=1; j++)
	//	{
	//		weight[(i+1)*3+j+1] = 1.0/9;
	//	}
	double weight[25] = {
		//0.1, 0.2,0.4,0.2,0.1
		0.01, 0.02, 0.04, 0.02, 0.01,
		0.02, 0.04, 0.08, 0.04, 0.02, 
		0.04, 0.08, 0.16, 0.08, 0.04, 
		0.02, 0.04, 0.08, 0.04, 0.02, 
		0.01, 0.02, 0.04, 0.02, 0.01
	};

	for (unsigned long line = 0; line < bmp.Height; ++line)
	{
		for (unsigned long row = 0; row < bmp.Width; ++row)
		{
			int radius = 2;
			int diameter = radius * 2 + 1;
			unsigned long *lpPixelBuffer = (unsigned long*) XL_GetBitmapBuffer(hBitmap, row, line);
			unsigned long *lpPixelBufferMatrix = new unsigned long [diameter*diameter];
			double redSum = 0;
			double greenSum = 0;
			double blueSum = 0;
			double alphaSum = 0;
			for ( int i = -radius; i <= radius; i++)
			{
				for (int j = -radius; j <= radius; j++)
				{
					lpPixelBufferMatrix[(i+radius)*diameter+(j+radius)] = 0;
					if (line + i >= 0 && line + i <bmp.Height && row + j >= 0 && row +j <bmp.Width)
						lpPixelBufferMatrix[(i+radius)*diameter+(j+radius)] = *((unsigned long*) XL_GetBitmapBuffer(hBitmap, row+j, line+i));
					else if (line + i < 0 && row + j >= 0 && row +j <bmp.Width)
						lpPixelBufferMatrix[(i+radius)*diameter+(j+radius)] = *((unsigned long*) XL_GetBitmapBuffer(hBitmap, row+j, 0));
					else if (line + i >= bmp.Height && row + j >= 0 && row +j <bmp.Width)
						lpPixelBufferMatrix[(i+radius)*diameter+(j+radius)] = *((unsigned long*) XL_GetBitmapBuffer(hBitmap, row+j, bmp.Height-1));
					else if (row + j < 0 && line + i >= 0 && line + i <bmp.Height)
						lpPixelBufferMatrix[(i+radius)*diameter+(j+radius)] = *((unsigned long*) XL_GetBitmapBuffer(hBitmap, 0, line + i));
					else if (row + j >= bmp.Width && line + i >= 0 && line + i <bmp.Height)
						lpPixelBufferMatrix[(i+radius)*diameter+(j+radius)] = *((unsigned long*) XL_GetBitmapBuffer(hBitmap, bmp.Width-1, line + i));
					else if (row + j < 0 && line + i < 0)
						lpPixelBufferMatrix[(i+radius)*diameter+(j+radius)] = *((unsigned long*) XL_GetBitmapBuffer(hBitmap, 0, 0));
					else if (row + j >= bmp.Width && line + i >= bmp.Height)
						lpPixelBufferMatrix[(i+radius)*diameter+(j+radius)] = *((unsigned long*) XL_GetBitmapBuffer(hBitmap,bmp.Width-1, bmp.Height-1));
					else 
						assert(false);
					
					unsigned long buffer = lpPixelBufferMatrix[(i+radius)*diameter+(j+radius)];
					unsigned int alpha = getA(buffer);
					unsigned int green = getG(buffer);
					unsigned int red = getR(buffer);
					unsigned int blue = getB(buffer);
					redSum += red * weight[(i+radius)*diameter+(j+radius)];
					greenSum += green * weight[(i+radius)*diameter+(j+radius)];
					blueSum += blue * weight[(i+radius)*diameter+(j+radius)];
					alphaSum += alpha * weight[(i+radius)*diameter+(j+radius)];
				}
			}
			unsigned int alpha = alphaSum;
			unsigned int green = greenSum;
			unsigned int red = redSum;
			unsigned int blue = blueSum;
			unsigned long temp = (alpha<<24)|(red<<16)|(green<<8)|blue;
			*lpPixelBuffer = temp;
		}
	}
}