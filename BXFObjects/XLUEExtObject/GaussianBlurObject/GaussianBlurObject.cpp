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
, m_type(TwoDimention)
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

	if (m_radius > 0 && m_sigma >0)
	{
		if (m_type == TwoDimention)
			Simple(hClipBitmap);
		else if (m_type == OneDimention)
			OneDimentionRender(hClipBitmap);
	}

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
	double watch[121];
	for (int i = 0; i < D*D; i++)
	{
		(*results)[i] /= sum;
		watch[i] = (*results)[i];
	}
}

void GaussianBlurObject::OneDimentionRender(XL_BITMAP_HANDLE hBitmap)const
{
	assert(hBitmap);
	XLBitmapInfo bmp;
	XL_GetBitmapInfo(hBitmap, &bmp);

	assert(bmp.ColorType == XLGRAPHIC_CT_ARGB32);

	int diameter = m_radius * 2 + 1;
	double *weight;
	GaussianFunction(m_sigma, m_radius, &weight);

	unsigned long *bufferLine;
	for (int line = 0; line < bmp.Height; ++line)
	{
		bufferLine = new unsigned long [bmp.Width];
		for (int col = 0; col < bmp.Width; ++col)
		{
			unsigned long *lpPixelBuffer = (unsigned long*) XL_GetBitmapBuffer(hBitmap, col, line);
			bufferLine[col] = *lpPixelBuffer;
			double redSum = 0;
			double greenSum = 0;
			double blueSum = 0;
			double alphaSum = 0;
			for (int j = -m_radius; j <= m_radius; j++)
			{
				unsigned long pixelBuffer;
				//Need read pixel @ line, col+j, if it's out of boundary, read boundary pixel
				if (j > 0) // forward pixel, not loaded to bufferLine yet, read from read bmp buffer
				{
					if (col +j < bmp.Width)
						pixelBuffer = *(unsigned long*) XL_GetBitmapBuffer(hBitmap, col+j, line);
					else 
						pixelBuffer = *(unsigned long*) XL_GetBitmapBuffer(hBitmap, bmp.Width - 1, line);
				}
				else if (j <= 0) // back pixel, already loaded, read from bufferLine
				{
					if (col + j >= 0)
						pixelBuffer = bufferLine[col+j];
					else
						pixelBuffer = bufferLine[0];
				}
				unsigned int alpha = getA(pixelBuffer);
				unsigned int green = getG(pixelBuffer);
				unsigned int red = getR(pixelBuffer);
				unsigned int blue = getB(pixelBuffer);
				redSum += red * weight[j+m_radius];
				greenSum += green * weight[j+m_radius];
				blueSum += blue * weight[j+m_radius];
				alphaSum += alpha * weight[j+m_radius];
			}
			unsigned int alpha = alphaSum;
			unsigned int green = greenSum;
			unsigned int red = redSum;
			unsigned int blue = blueSum;
			unsigned long temp = (alpha<<24)|(red<<16)|(green<<8)|blue;
			*lpPixelBuffer = temp;
		}
		delete []bufferLine;
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
	unsigned long **lpPixelBufferLines = new unsigned long* [bmp.Height];

	for (int preloadLine = 0; preloadLine < m_radius; preloadLine++)
	{
		lpPixelBufferLines[preloadLine] = new unsigned long[bmp.Width];
		for (int preloadCol = 0; preloadCol < bmp.Width; preloadCol++)
		{
			unsigned long *lpPixelBuffer = (unsigned long*) XL_GetBitmapBuffer(hBitmap, preloadCol, preloadLine); 
			lpPixelBufferLines[preloadLine][preloadCol] = *lpPixelBuffer;
		}
	}

	for (int line = 0; line < bmp.Height; ++line)
	{
		if ( line >= m_radius + 1)
		{
			delete []lpPixelBufferLines[line - m_radius-1];
		}
		if (line + m_radius < bmp.Height)
			lpPixelBufferLines[line + m_radius] = new unsigned long[bmp.Width];

		for (int col = 0; col < bmp.Width; ++col)
		{
			unsigned long *lpPixelBuffer = (unsigned long*) XL_GetBitmapBuffer(hBitmap, col, line);
			if (col + m_radius < bmp.Width && line + m_radius < bmp.Height)
			{
				unsigned long *lpPixelBufferPreload = (unsigned long*) XL_GetBitmapBuffer(hBitmap, col+m_radius, line+m_radius);
				lpPixelBufferLines[line + m_radius][col + m_radius] = *lpPixelBufferPreload;
			}
			double redSum = 0;
			double greenSum = 0;
			double blueSum = 0;
			double alphaSum = 0;
			for ( int i = -m_radius; i <= m_radius; i++)
			{
				for (int j = -m_radius; j <= m_radius; j++)
				{
					unsigned long pixelBuffer = 0;

					pixelBuffer = lpPixelBufferLines[line][col];
					if (i > 0) // 读XL_GetBitmapBuffer
					{
						if (line + i <bmp.Height && col + j >= 0 && col +j <bmp.Width)
						{
							pixelBuffer = *((unsigned long*) XL_GetBitmapBuffer(hBitmap, col+j, line+i));
						}
						else if ( line + i <bmp.Height && col + j < 0)
						{
							pixelBuffer = *((unsigned long*) XL_GetBitmapBuffer(hBitmap, 0, line + i));
						}
						else if ( line + i <bmp.Height && col + j >= bmp.Width)
						{
							pixelBuffer = *((unsigned long*) XL_GetBitmapBuffer(hBitmap, bmp.Width-1, line + i));
						}
						else if (line + i >= bmp.Height && col + j >= 0 && col +j <bmp.Width)
						{
							pixelBuffer = *((unsigned long*) XL_GetBitmapBuffer(hBitmap, col+j, bmp.Height-1));
						}
						else if (line + i >= bmp.Height && col + j >= bmp.Width)
						{
							pixelBuffer = *((unsigned long*) XL_GetBitmapBuffer(hBitmap,bmp.Width-1, bmp.Height-1));
						}
						else if (line + i >= bmp.Height && col + j < 0)
						{
							pixelBuffer = *((unsigned long*) XL_GetBitmapBuffer(hBitmap, 0, bmp.Height - 1));
						}
						else 
							assert(false);
					}
					else if (i < 0) // 读 lpPixelBufferRows的数据, 因为Bitmap的数据已经被改写了
					{
						if (line + i >= 0 && col + j >= 0 && col +j <bmp.Width)
						{ 
								pixelBuffer = lpPixelBufferLines[line+i][col+j];
						}
						else if (line + i >= 0 && col + j < 0)
						{
							pixelBuffer =lpPixelBufferLines[line + i][0]; 
						}
						else if (line + i >= 0 && col + j >= bmp.Width)
						{
							pixelBuffer = lpPixelBufferLines[line+i][bmp.Width-1]; 
						}
						else if (line + i < 0 && col + j >= 0 && col +j <bmp.Width)
						{
							pixelBuffer = lpPixelBufferLines[0][col+j]; 
						}
						else if ( line + i < 0 && col + j < 0)
						{
							pixelBuffer = lpPixelBufferLines[0][0]; 
						}
						else if (line + i < 0 && col + j >= bmp.Width)
						{
							pixelBuffer = lpPixelBufferLines[0][bmp.Width - 1];
						}
						else 
							assert(false);
					} 
					else //if i == 0, line + i is sure valid
					{
						if (j > 0)
						{
							if (col + j >= 0 && col +j <bmp.Width)
							{
								pixelBuffer = *((unsigned long*) XL_GetBitmapBuffer(hBitmap, col+j, line+i));
							}
							else if (col + j < 0)
							{
								pixelBuffer = *((unsigned long*) XL_GetBitmapBuffer(hBitmap, 0, line + i));
							}
							else if (col + j >= bmp.Width)
							{
								pixelBuffer = *((unsigned long*) XL_GetBitmapBuffer(hBitmap, bmp.Width-1, line + i));
							}
							else
								assert (false);
						}
						else // j <= 0, already loaded in pixelBuffer
						{
							if (col + j >= 0 && col +j <bmp.Width)
							{ 
									pixelBuffer = lpPixelBufferLines[line+i][col+j];
							}
							else if (col + j < 0)
							{
								pixelBuffer =lpPixelBufferLines[line + i][0]; 
							}
							else if (col + j >= bmp.Width)
							{
								pixelBuffer = lpPixelBufferLines[line+i][bmp.Width-1]; 
							}
							else
								assert (false);
						}
					}
					
					unsigned int alpha = getA(pixelBuffer);
					unsigned int green = getG(pixelBuffer);
					unsigned int red = getR(pixelBuffer);
					unsigned int blue = getB(pixelBuffer);
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
			*lpPixelBuffer = (alpha<<24)|(red<<16)|(green<<8)|blue;
		}
	}
	for (unsigned long bufferLine = bmp.Height - m_radius - 1; bufferLine < bmp.Height; bufferLine++)
	{
			delete []lpPixelBufferLines[bufferLine];
	}
	delete []lpPixelBufferLines;
}