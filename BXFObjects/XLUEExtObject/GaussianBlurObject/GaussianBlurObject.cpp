/********************************************************************
/* Copyright (c) 2013 The BOLT UIEngine. All rights reserved.
/* Use of this source code is governed by a BOLT license that can be
/* found in the LICENSE file.
********************************************************************/ 
#include "stdafx.h"
#include "./GaussianBlurObject.h"
#include <cmath>
#include <iostream>
#include <time.h>

#include <mmintrin.h>  //MMX
#include <xmmintrin.h> //SSE
#include <emmintrin.h> //SSE2
#include <pmmintrin.h> //SSE3
#include <tmmintrin.h> //SSSE3
#include <smmintrin.h> //SSE4.1
#include <nmmintrin.h> //SSE4.2
#include <omp.h>
GaussianBlurObject::GaussianBlurObject( XLUE_LAYOUTOBJ_HANDLE hObj )
:ExtLayoutObjMethodsImpl(hObj)
, m_sigma(0)
, m_radius(1)
, m_type(DirecheIIR)
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
{__m128 test;
	clock_t time1 = clock();
	assert(lpSrcClipRect);
	const RECT* pos = GetPos();
//	assert(RectHelper::EqualRect(lpSrcClipRect, GetPos()));

	XL_BITMAP_HANDLE hClipBitmap = XL_ClipSubBindBitmap(hBitmapDest, lpDestClipRect);
	assert(hClipBitmap);

	if (m_radius >= 0 && m_sigma >0)
	{
		if (m_type == TwoDimention)
		{
			Simple(hClipBitmap);
		}
		// FIR 型滤波
		else if (m_type == OneDimention)
		{
			OneDimentionRender(hClipBitmap);
		}
		else if (m_type == DirecheIIR)
		{
			DericheIIRRender(hClipBitmap);
		}
	}

	XL_ReleaseBitmap(hClipBitmap);
	clock_t time2=clock();
	float diff = (((float)time2 - (float)time1) / 1000000.0F ) * 1000; 
}
/* Calcualte Gaussian Blur Filter Coefficiens
 *  alpha -> smooting gradient depends on sigma
 *  k = ((1-e^-alpha)^2)/(1+2*alpha*e^-alpha - e^-2alpha)
 *  a0 = k; a1 = k*(alpha-1)*e^-alpha; a2 = k*(alpha+1)*e^-alpha; a3 = -k*e^(-2*alpha)
 *  b1 = -2*e^-alpha; b2 = e^(-2*alpha)
 */
void calGaussianCoeff( float sigma,  float *a0, float *a1, float *a2, float *a3, float *b1, float *b2, float *cprev, float *cnext)
{
  float alpha, lamma,  k; 
  // defensive check
  if (sigma < 0.5f)
	  sigma = 0.5f;

  alpha = (float) exp((0.726)*(0.726)) / sigma;
  lamma = (float)exp(-alpha);
  *b2 = (float)exp(-2*alpha);
  k = (1-lamma)*(1-lamma)/(1+2*alpha*lamma- (*b2));
  *a0 = k;
  *a1 = k*(alpha-1)*lamma;
  *a2 = k*(alpha+1)*lamma;
  *a3 = -k* (*b2);
  *b1 = -2*lamma;
  *cprev = (*a0 + *a1)/(1+ *b1 + *b2);
  *cnext = (*a2 + *a3)/(1+ *b1 + *b2);
}
inline unsigned int getR(const unsigned long & buffer)
{
	return buffer<<8>>24;
}
inline unsigned int getG(const unsigned long & buffer)
{
	return buffer<<16>>24;
}
inline unsigned int getB(const unsigned long & buffer)
{
	return buffer<<24>>24;
}
inline unsigned int getA(const unsigned long & buffer)
{
	return buffer>>24;
}
/* SSE Implementation: gaussianHorizontal_sse
 *		oTemp - Temporary small buffer used between left to right pass
 *		id    - input image 
 *		od    - output image from this filter
 *		height - image height
 *		width - image original width
 *		Nwidth - Padded width
 * 		a0, a1, a2, a3, b1, b2, cprev, cnext: Gaussian coefficients
 */
inline void assignLongTo4Floats(float* out, unsigned long *in)
{
	unsigned int alpha = getA(*in);
	unsigned int green = getG(*in);
	unsigned int red = getR(*in);
	unsigned int blue = getB(*in);
	out[0] = (double)blue;
	out[1] = (double)green;
	out[2] = (double)red;
	out[3] = (double)alpha;
}

inline void assign4FloatsToLong(unsigned long *out, float *in)
{
	unsigned int blue= in[0];
	unsigned int green = in[1];
	unsigned int red = in[2];
	unsigned int alpha = in[3];
	*out = (alpha<<24)|(red<<16)|(green<<8)|blue;
}

inline void multi4Floats(float *out, float *in, float *coeff)
{
	out[0] = in[0] * (*coeff);
	out[1] = in[1] * (*coeff);
	out[2] = in[2] * (*coeff);
	out[3] = in[3] * (*coeff);
}
inline void assign4Floats(float *out, float *in)
{
	out[0] = in[0];
	out[1] = in[1];
	out[2] = in[2];
	out[3] = in[3];
}
inline void add4Floats(float *out, float *in_l, float *in_r)
{
	out[0] = in_l[0] + in_r[0];
	out[1] = in_l[1] + in_r[1];
	out[2] = in_l[2] + in_r[2];
	out[3] = in_l[3] + in_r[3];
}
inline void sub4Floats(float *out, float *in_l, float *in_r)
{
	out[0] = in_l[0] - in_r[0];
	out[1] = in_l[1] - in_r[1];
	out[2] = in_l[2] - in_r[2];
	out[3] = in_l[3] - in_r[3];
}
/*
	Input:
	00,01,02
	10,11,12

	Output:
	00,10
	01,11
	02,12

	width = 3;
	height = 2
*/
/*
	//for (int col = 0; col < width; col++)
	//{
	//	assignLongTo4Floats(od, id);
	//	id += 1;
	//	od += height*4;
	//}
	//return;
*/
void DerichIIRHorizontal(float *oTemp,  unsigned long* id, float *od, int width, int height, int Nwidth, float *a0, float *a1, float *a2, float *a3, float *b1, float *b2, float *cprev, float *cnext)
{
	float prevIn[4];
	float currIn[4];
	float prevOut[4];
	float prev2Out[4];

	float currComp[4];
	float temp1[4];
	float temp2[4];
	float temp3[4];

	// 第一遍从左往右的公式是: oTemp[i] = (a0*id[i] + a1*id[i-1]) - (b1*oTemp[i-1] + b2*oTemp[i-2])
	// 第二遍从右往左的公式是: od[i] = oTemp[i] + (a3*id[i+1] + a4*id[i+2]) - (b1*od[i+1]+b2*od[i+2])
	assignLongTo4Floats(prevIn, id);
	multi4Floats(prevOut, prevIn, cprev);
	assign4Floats(prev2Out, prevOut);
	for (int x = 0; x < width; ++x)
	{
		assignLongTo4Floats(currIn, id);
		multi4Floats(currComp, currIn, a0);
		multi4Floats(temp1, prevIn, a1);
		multi4Floats(temp2, prevOut, b1);
		multi4Floats(temp3, prev2Out, b2);
		add4Floats(currComp, currComp, temp1);
		add4Floats(temp2, temp2, temp3);
		assign4Floats(prev2Out, prevOut);
		sub4Floats(prevOut, currComp, temp2);
		assign4Floats(prevIn, currIn);

		assign4Floats(oTemp, prevOut);
		oTemp+=4;
		id+=1;
	}
	id -= 1;
	od += 4*height*(width-1);// 最后一行行首
	oTemp -= 4;
	
	assignLongTo4Floats(prevIn, id);
	multi4Floats(prev2Out, prevIn, cnext);
	assign4Floats(prevOut, prev2Out);
	assign4Floats(currIn, prevIn);

	a0 = a2;
	a1 = a3;

	float inNext[4];
	float output[4];

	for (int x = width - 1; x >= 0; --x)
	{
		assignLongTo4Floats(inNext, id);
		assign4Floats(output, oTemp);

		multi4Floats(currComp, currIn, a0);
		multi4Floats(temp1, prevIn, a1);
		multi4Floats(temp2, prevOut, b1);
		multi4Floats(temp3, prev2Out, b2);
		add4Floats(currComp, currComp, temp1);
		add4Floats(temp2, temp2, temp3);
		assign4Floats(prev2Out, prevOut);
		sub4Floats(prevOut, currComp, temp2);
		assign4Floats(prevIn, currIn);
		assign4Floats(currIn, inNext);
		add4Floats(output, output, prevOut);

		assign4Floats(od, output);
		id -= 1;
		od -= 4*height;
		oTemp -= 4;
	}
}

/*
	Input:
	00(id),	10
	01,		11
	02,		12

	Output:
	00(od),	01,02
	10,		11,12

	width:2
	height:3
*/
/*
	for (int col = 0; col < height; col++)
	{
		assign4FloatsToLong(od, id);
		od += width;
		id += 4;
	}
	return;
	*/
void DerichIIRVertical(float *oTemp, float *id, unsigned long *od, int height, int width, float *a0, float *a1, float *a2, float *a3, float *b1, float *b2, float *cprev, float *cnext)
{
	float prevIn[4];
	float currIn[4];
	float prevOut[4];
	float prev2Out[4];

	float currComp[4];
	float temp1[4];
	float temp2[4];
	float temp3[4];

	assign4Floats(prevIn, id);
	multi4Floats(prev2Out, prevIn, cprev);
	assign4Floats(prevOut, prev2Out);

	for (int col = 0; col < height; col++)
	{
		assign4Floats(currIn, id);

		multi4Floats(currComp, currIn, a0);
		multi4Floats(temp1, prevIn, a1);
		multi4Floats(temp2, prevOut, b1);
		multi4Floats(temp3, prev2Out, b2);

		add4Floats(currComp, currComp, temp1);
		add4Floats(temp2, temp2, temp3);
		assign4Floats(prev2Out, prevOut);
		sub4Floats(prevOut, currComp, temp2);
		assign4Floats(prevIn, currIn);
		assign4Floats(oTemp, prevOut);

		oTemp += 4;
		id += 4;
	}
	id -= 4;
	oTemp -= 4;
	od += width*(height-1);

	a0 = a2;
	a1 = a3;
	assign4Floats(prevIn, id);
	assign4Floats(currIn, prevIn);
	multi4Floats(prev2Out, prevIn, cnext);
	assign4Floats(prevOut, prev2Out);

	float inNext[4];
	float output[4];
	for(int row = height - 1; row >= 0; row--)
	{
		assign4Floats(inNext, id);
		assign4Floats(output, oTemp);
		multi4Floats(currComp, currIn, a0);
		multi4Floats(temp1, prevIn, a1);
		multi4Floats(temp2, prevOut, b1);
		multi4Floats(temp3, prev2Out, b2);

		add4Floats(currComp, currComp, temp1);
		add4Floats(temp2, temp2, temp3);
		assign4Floats(prev2Out, prevOut);
		sub4Floats(prevOut, currComp, temp2);
		assign4Floats(prevIn, currIn);
		assign4Floats(currIn, inNext);

		add4Floats(output, output, prevOut);
		assign4FloatsToLong(od, output);
		
		id -= 4;
		od -= width;
		oTemp -= 4;
	}

}

void GaussianBlurObject::DericheIIRRender(XL_BITMAP_HANDLE hBitmap)const
{
	SYSTEM_INFO sysInfo;
	GetSystemInfo( &sysInfo );
	int nCPU = sysInfo.dwNumberOfProcessors;
	int threadNum = nCPU;
	omp_set_num_threads(threadNum);

	XLBitmapInfo bmp;
	XL_GetBitmapInfo(hBitmap, &bmp);

	assert(bmp.ColorType == XLGRAPHIC_CT_ARGB32);

	float a0, a1, a2, a3, b1, b2, cprev, cnext;
	calGaussianCoeff(m_sigma, &a0, &a1, &a2, &a3, &b1, &b2, &cprev, &cnext);

	unsigned long *lpPixelBufferInitial = (unsigned long*)XL_GetBitmapBuffer(hBitmap, 0, 0);
	unsigned long *lpRowInitial = (unsigned long*)XL_GetBitmapBuffer(hBitmap, 0, 1);

	int bufferSizePerThread = (bmp.Width>bmp.Height?bmp.Width:bmp.Height)*4;
	float *oTemp = new float[bufferSizePerThread*threadNum];
	float *od = new float[bmp.Width*bmp.Height*4];
	
#pragma omp parallel for 
	for (int row = 0; row < bmp.Height; ++row)
	{
		int tidx = omp_get_thread_num();
		unsigned long *lpRowInitial = lpPixelBufferInitial + bmp.ScanLineLength/4*row;
		float *lpColumnInitial = od + row*4;
		float *oTempThread = oTemp + bufferSizePerThread * tidx;
		DerichIIRHorizontal(oTempThread, lpRowInitial, lpColumnInitial, bmp.Width, bmp.Height, bmp.Width, &a0, &a1, &a2, &a3, &b1, &b2, &cprev, &cnext );
	}

#pragma omp parallel for
	for (int col = 0; col < bmp.Width; ++col)
	{
		int tidx = omp_get_thread_num();
		/*
		原图buffer作为输出指针
		00(COL=0),	01(col=1),	02(col=2)
		10,			11,			12
		*/
		unsigned long *lpColInitial = lpPixelBufferInitial + col;
		//unsigned long *lpColInitial = (unsigned long*)XL_GetBitmapBuffer(hBitmap, col, 0);
		/*
		od作为输入指针
		00, 10
		01, 11
		02, 12
		*/
		float *lpRowInitial = od+bmp.Height*col*4;
		float *oTempThread = oTemp + bufferSizePerThread * tidx;
		//DerichIIRVertical(float *oTemp, float *id, unsigned long *od, int width, int height, float *a0, float *a1, float *a2, float *a3, float *b1, float *b2, float *cprev, float *cnext)
		DerichIIRVertical(oTempThread, lpRowInitial, lpColInitial, bmp.Height, bmp.ScanLineLength/4, &a0, &a1, &a2, &a3, &b1, &b2, &cprev, &cnext);
	}

	delete []oTemp;
	delete []od;
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

	unsigned long *lpPixelBufferLine;
	unsigned long *lpPixelBufferInitial = (unsigned long*)XL_GetBitmapBuffer(hBitmap, 0, 0);

	for (int line = 0; line < bmp.Height; ++line)
	{
		lpPixelBufferLine = new unsigned long [bmp.Width];
		memcpy(lpPixelBufferLine, lpPixelBufferInitial + bmp.ScanLineLength/4*line, bmp.Width * 4);
		for (int col = 0; col < bmp.Width; ++col)
		{
			unsigned long *lpPixelBuffer = lpPixelBufferInitial + bmp.ScanLineLength/4*line + col;
			double redSum = 0;
			double greenSum = 0;
			double blueSum = 0;
			double alphaSum = 0;
			for (int j = -m_radius; j <= m_radius; j++)
			{
				unsigned long pixelBuffer;
				//Need read pixel @ line, col+j, if it's out of boundary, read boundary pixel
				if (col + j < 0)
				{
					pixelBuffer = lpPixelBufferLine[0];
				}
				else if (col + j >= bmp.Width)
				{
					pixelBuffer = lpPixelBufferLine[bmp.Width - 1];
				}
				else
				{
					pixelBuffer = lpPixelBufferLine[col + j];
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
			*lpPixelBuffer = (alpha<<24)|(red<<16)|(green<<8)|blue;
		}
		delete []lpPixelBufferLine;
	}
	for (int column = 0; column < bmp.Width; ++column)
	{
		lpPixelBufferLine = new unsigned long [bmp.Height];
		for (int row = 0; row < m_radius; ++row)
		{
			lpPixelBufferLine[row] = *(lpPixelBufferInitial + bmp.ScanLineLength/4*row + column);
		}
		for (int row = 0; row < bmp.Height; ++row)
		{
			if (row + m_radius < bmp.Height)
			{
				lpPixelBufferLine[row+m_radius] = *(lpPixelBufferInitial + bmp.ScanLineLength/4*(row+m_radius)+column);
			}
			unsigned long *lpPixelBuffer = lpPixelBufferInitial + bmp.ScanLineLength/4*row + column;
			double redSum = 0;
			double greenSum = 0;
			double blueSum = 0;
			double alphaSum = 0;
			for (int j = -m_radius; j <= m_radius; j++)
			{
				unsigned long pixelBuffer;
				if (row + j < 0)
				{
					pixelBuffer = lpPixelBufferLine[0];
				}
				else if (row + j >= bmp.Height)
				{
					pixelBuffer = lpPixelBufferLine[bmp.Height-1];
				}
				else
				{
					pixelBuffer = lpPixelBufferLine[row+j];
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
			*lpPixelBuffer = (alpha<<24)|(red<<16)|(green<<8)|blue;
		}
		delete []lpPixelBufferLine;
	}
}

/*
memcpy bmp.Height 次调用在bmp.Width * 4长度上
XL_GetBitmapBuffer 1次
*/
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
	unsigned long *lpPixelBufferInitial = (unsigned long*)XL_GetBitmapBuffer(hBitmap, 0,0);
	long lineWidth = bmp.ScanLineLength;
	// todo: 如果位图是倒叙存储的应该怎样处理? 

	for (int preloadLine = 0; preloadLine < m_radius; preloadLine++)
	{
		lpPixelBufferLines[preloadLine] = new unsigned long[bmp.Width];
		unsigned long *lpPixelBufferSourceLine = lpPixelBufferInitial + lineWidth/4 * preloadLine;
		// TODO, pixel buffer size should not be fixed 4 bytes
		memcpy(lpPixelBufferLines[preloadLine], lpPixelBufferSourceLine, bmp.Width * 4);
	}

	for (int line = 0; line < bmp.Height; ++line)
	{
		// bitmap这一行的内存首地址
		unsigned long *lpPixelBufferBmpLine = lpPixelBufferInitial + lineWidth/4 * line;
		// 把m_radius行之前的buffer释放掉, 因为后面都用不到了
		if ( line >= m_radius + 1)
		{
			delete []lpPixelBufferLines[line - m_radius-1];
		}
		// 提前加载m_radius行之后的line, 因为要用到
		if (line + m_radius < bmp.Height)
		{
			lpPixelBufferLines[line + m_radius] = new unsigned long[bmp.Width];
			unsigned long *lpPixelBufferBmpLinePlusRadius = lpPixelBufferInitial + lineWidth/4 * (line + m_radius);
			memcpy(lpPixelBufferLines[line + m_radius], lpPixelBufferBmpLinePlusRadius, bmp.Width * 4);
		}

		for (int col = 0; col < bmp.Width; ++col)
		{
			unsigned long *lpPixelBuffer = lpPixelBufferBmpLine + col;
			double redSum = 0;
			double greenSum = 0;
			double blueSum = 0;
			double alphaSum = 0;
			for ( int i = -m_radius; i <= m_radius; i++)
			{
				for (int j = -m_radius; j <= m_radius; j++)
				{
					unsigned long pixelBuffer = 0;
					unsigned int adjustedLine, adjustedCol;
					if (line + i >= (int)bmp.Height)
					{
						adjustedLine = bmp.Height - 1;
					}
					else if (line + i < 0)
					{
						adjustedLine = 0;
					}
					else
					{
						adjustedLine = line + i;
					}
					if (col + j >= (int)bmp.Width)
					{
						adjustedCol = bmp.Width - 1;
					}
					else if (col + j < 0)
					{
						adjustedCol = 0;
					}
					else
					{
						adjustedCol = col + j;
					}
					pixelBuffer = lpPixelBufferLines[adjustedLine][adjustedCol];

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