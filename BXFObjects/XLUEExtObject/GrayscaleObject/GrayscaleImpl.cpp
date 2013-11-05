/********************************************************************
/* Copyright (c) 2013 The BOLT UIEngine. All rights reserved.
/* Use of this source code is governed by a BOLT license that can be
/* found in the LICENSE file.
********************************************************************/ 
#include "stdafx.h"
#include "./GrayscaleImpl.h"
#include <cmath>
#include <omp.h>

extern "C" void Averaging_MMX_line(unsigned long *lpPixelBufferLine);
void Averaging_C_Pixel(unsigned long *lpPixelBuffer);
void Averaging_2_Pixel(unsigned long *lpPixelBuffer);

void AveragingGrayscale(XL_BITMAP_HANDLE hBitmap)
{	
	assert(hBitmap);
	XLBitmapInfo bmp;
	XL_GetBitmapInfo(hBitmap, &bmp);

	assert(bmp.ColorType == XLGRAPHIC_CT_ARGB32);
	
	unsigned long *lpPixelBufferInitial = (unsigned long*)XL_GetBitmapBuffer(hBitmap, 0, 0);
	int scanLineLengthInPixel = bmp.ScanLineLength / 4;

#pragma omp parallel for 
	for (int line = 0; line < bmp.Height; ++line)
	{
		unsigned long *lpPixelBuffer = lpPixelBufferInitial + scanLineLengthInPixel * line;
		for (int col = 0; col < bmp.Width/2; col++)
		{
			Averaging_2_Pixel(lpPixelBuffer);
			lpPixelBuffer +=2;
		}
	}
}

unsigned long mask[] = {0x00ff0000, 0x00ff0000};
unsigned long factor[]= {342, 342};
// 介个一次计算俩
void Averaging_2_Pixel(unsigned long *lpPixelBuffer)
{
	_asm
	{	
		mov esi, dword ptr [lpPixelBuffer];

		movq mm2, dword ptr [mask];
		movq mm3, dword ptr [factor];

		movq mm0, [esi];
		movq mm6, mm0;
		pand mm0, mm2;
		psrld mm0, 10h;
		movq mm1, mm0;

		movq mm0, mm6;
		psrld mm2, 8h;
		pand mm0, mm2;
		psrld mm0, 8h;
		paddd mm1, mm0;

		movq mm0, mm6;
		psrld mm2, 8h;
		pand mm0, mm2;
		paddd mm1, mm0;

		movq mm4, mm1;
		pmullw mm1, mm3;
		pmulhw mm4, mm3;
		pslld mm4, 10h;
		paddd mm1, mm4;
		psrld mm1, 10; Now mm1 = 0x000000gray000000gray'

		movq mm5, mm1;
		pslld mm5, 8h;
		paddd mm1, mm5;
		
		pslld mm5, 8h;
		paddd mm1, mm5;

		pslld mm2, 24;
		pand mm6, mm2;

		paddd mm1, mm6;
		movq [esi], mm1;

		emms;
	}
}

// 介个一次计算一个像素
void Averaging_C_Pixel(unsigned long *lpPixelBuffer)
{
	_asm
	{	
		mov esi, dword ptr [lpPixelBuffer];

		mov ecx, [esi];
		and ecx, 0x00ff0000;
		shr ecx, 10h;
		mov eax, ecx;
		mov ecx, [esi];
		and ecx, 0x0000ff00;
		shr ecx, 8h;
		add eax, ecx;
		mov ecx, [esi];
		and ecx, 0x000000ff;
		add eax, ecx;

		imul eax, 341;
		shr eax, 10;
		add eax, 1;

		;mov ecx, 3;
		;xor edx, edx;
		;div ecx;
		
		mov ecx, [esi];
		and ecx, 0xff000000;
		add ecx, eax;
		shl eax, 8h;
		add ecx, eax;
		shl eax, 8h;
		add ecx, eax;
		mov [esi], ecx;

		emms;
	}
}