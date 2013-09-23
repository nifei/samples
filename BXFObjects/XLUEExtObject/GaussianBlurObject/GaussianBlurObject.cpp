/********************************************************************
/* Copyright (c) 2013 The BOLT UIEngine. All rights reserved.
/* Use of this source code is governed by a BOLT license that can be
/* found in the LICENSE file.
********************************************************************/ 
#include "stdafx.h"
#include "./GaussianBlurObject.h"
#include <cmath>
#include <iostream>

GaussianBlurObject::GaussianBlurObject( XLUE_LAYOUTOBJ_HANDLE hObj )
:ExtLayoutObjMethodsImpl(hObj)
, m_sigma(0)
, m_radius(1)
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

	if (m_radius > 1 && m_sigma >0)
		Simple(hClipBitmap);

	XL_ReleaseBitmap(hClipBitmap);
}

const double pi = 3.14159265358979323846;
void GaussianFunction(double sigma, int r, double ** results)
{
	*results =new double[r*2+1];
	double sum = 0;
	double factor = 1.0/sigma/sqrt(2*pi);
	for ( int i=0; i < r+1; i++)
	{
		(*results)[i] = exp(0- (i-r)*(i-r)/(2*sigma*sigma)) * factor;
		sum += (*results)[i];
	}
	for (int i = r+1; i < r*2+1; i++)
	{
		(*results)[i] = (*results)[r*2-i];
		sum += (*results)[i];
	}
	assert(sum>0);
	double temp[49];
	for (int i = 0; i < r*2+1; i++)
	{
		(*results)[i] /= sum;
		temp[i] = (*results)[i];
	}
}

void GaussianFunction2(double sigma, int r, double **results)
{
	int D = r*2+1;
	*results = new double[D*D];
	double sum = 0;
	double factor = 1.0/2/pi/sigma/sigma;
	for ( int i = 0;i < r + 1; i++)
	{
		for (int j = 0; j < r+1; j++)
		{
			(*results)[i*D+j] = exp(0 - ((i-r)*(i-r) + (j-r)*(j-r))/(2*sigma*sigma)) * factor; 
			sum += (*results)[i*D+j];
		}
	}
	for (int i = r+1; i < D; i++)
	{
		for (int j= 0; j < r+1; j++)
		{
			(*results)[i*D+j] = (*results)[(r*2-i)*D+j];
			sum += (*results)[i*D+j];
		}
	}
	for (int i = 0; i < r+1; i++)
	{
		for (int j = r+1; j < D; j++)
		{
			(*results)[i*D+j] = (*results)[i*D+(r*2-j)];
			sum += (*results)[i*D+j];
		}
	}
	for (int i = r+1; i<D; i++)
	{
		for (int j = r+1; j< D; j++)
		{
			(*results)[i*D+j] = (*results)[(r*2-i)*D+(r*2-j)];
			sum += (*results)[i*D+j];
		}
	}
	for (int i = 0; i < D*D; i++)
	{
		(*results)[i] /= sum;
	}
}

void GaussianBlurObject::Simple(XL_BITMAP_HANDLE hBitmap)const
{
	assert(hBitmap);
	XLBitmapInfo bmp;
	XL_GetBitmapInfo(hBitmap, &bmp);

	assert(bmp.ColorType == XLGRAPHIC_CT_ARGB32);

	int diameter = m_radius * 2 + 1;
	double *weight;
	GaussianFunction2(m_sigma, m_radius, &weight);

	for (unsigned long line = 0; line < bmp.Height; ++line)
	{
		for (unsigned long row = 0; row < bmp.Width; ++row)
		{
			unsigned long *lpPixelBuffer = (unsigned long*) XL_GetBitmapBuffer(hBitmap, row, line);
			unsigned long *lpPixelBufferMatrix = new unsigned long [diameter*diameter];
			double redSum = 0;
			double greenSum = 0;
			double blueSum = 0;
			double alphaSum = 0;
			for ( int i = -m_radius; i <= m_radius; i++)
			{
				for (int j = -m_radius; j <= m_radius; j++)
				{
					lpPixelBufferMatrix[(i+m_radius)*diameter+(j+m_radius)] = 0;
					if (line + i >= 0 && line + i <bmp.Height && row + j >= 0 && row +j <bmp.Width)
						lpPixelBufferMatrix[(i+m_radius)*diameter+(j+m_radius)] = *((unsigned long*) XL_GetBitmapBuffer(hBitmap, row+j, line+i));
					else if (line + i < 0 && row + j >= 0 && row +j <bmp.Width)
						lpPixelBufferMatrix[(i+m_radius)*diameter+(j+m_radius)] = *((unsigned long*) XL_GetBitmapBuffer(hBitmap, row+j, 0));
					else if (line + i >= bmp.Height && row + j >= 0 && row +j <bmp.Width)
						lpPixelBufferMatrix[(i+m_radius)*diameter+(j+m_radius)] = *((unsigned long*) XL_GetBitmapBuffer(hBitmap, row+j, bmp.Height-1));
					else if (row + j < 0 && line + i >= 0 && line + i <bmp.Height)
						lpPixelBufferMatrix[(i+m_radius)*diameter+(j+m_radius)] = *((unsigned long*) XL_GetBitmapBuffer(hBitmap, 0, line + i));
					else if (row + j >= bmp.Width && line + i >= 0 && line + i <bmp.Height)
						lpPixelBufferMatrix[(i+m_radius)*diameter+(j+m_radius)] = *((unsigned long*) XL_GetBitmapBuffer(hBitmap, bmp.Width-1, line + i));
					else if (row + j < 0 && line + i < 0)
						lpPixelBufferMatrix[(i+m_radius)*diameter+(j+m_radius)] = *((unsigned long*) XL_GetBitmapBuffer(hBitmap, 0, 0));
					else if (row + j >= bmp.Width && line + i >= bmp.Height)
						lpPixelBufferMatrix[(i+m_radius)*diameter+(j+m_radius)] = *((unsigned long*) XL_GetBitmapBuffer(hBitmap,bmp.Width-1, bmp.Height-1));
					else 
						assert(false);
					
					unsigned long buffer = lpPixelBufferMatrix[(i+m_radius)*diameter+(j+m_radius)];
					unsigned int alpha = getA(buffer);
					unsigned int green = getG(buffer);
					unsigned int red = getR(buffer);
					unsigned int blue = getB(buffer);
					redSum += red * weight[(i+m_radius)*diameter+(j+m_radius)];
					greenSum += green * weight[(i+m_radius)*diameter+(j+m_radius)];
					blueSum += blue * weight[(i+m_radius)*diameter+(j+m_radius)];
					alphaSum += alpha * weight[(i+m_radius)*diameter+(j+m_radius)];
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