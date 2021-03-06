/********************************************************************
/* Copyright (c) 2013 The BOLT UIEngine. All rights reserved.
/* Use of this source code is governed by a BOLT license that can be
/* found in the LICENSE file.
********************************************************************/ 
#include "stdafx.h"
#include "./GaussianBlurDelegate.h"
#include <cmath>
#include <omp.h>

const float pi = 3.14159265358979323846;

void GaussianFunctionInteger(float i_sigma, int & io_radius, short ** o_results, int shift)
{
	float *fWeights = new float[io_radius * 2 + 1];
	float fSum = 0;
	// float fFactor = 1.0/i_sigma/sqrt(2*pi); // later we'll scale the weights to sum in 2^shift, so fFactor is not necessary to sum up to 1
	for ( int i = 0; i < io_radius + 1; i++)
	{
		(fWeights)[i] = exp(0 - (i - io_radius) *(i - io_radius)/(2 * i_sigma * i_sigma));
		fSum += (fWeights)[i];
	}
	for (int i = io_radius + 1; i < io_radius * 2 + 1; i++)
	{
		(fWeights)[i] = (fWeights)[io_radius * 2 - i];
		fSum += (fWeights)[i];
	} // normal distribution weights

	int expectedSum = 1;
	while (shift > 0)
	{
		expectedSum *= 2;
		shift -= 1;
	}
	float tmpFactor = (float)expectedSum / fSum;
	int diameter = 2 * io_radius + 1;

	*o_results = new short[diameter];
	int sum = 0;
	int firstNonZero = -1;
	for (int i = 0; i < diameter; i++)
	{
		(*o_results)[i] = (short)(fWeights[i] * tmpFactor + 0.5);
		sum += (*o_results)[i];
		if ((*o_results)[i] > 0 && firstNonZero == -1)
		{
			firstNonZero = i;
		}
	}

	while(firstNonZero >= 1 && sum < expectedSum)
	{
		(*o_results)[firstNonZero - 1]++;
		(*o_results)[diameter-firstNonZero]++;
		sum += 2;
		firstNonZero--;
	}

	if (sum > expectedSum)
	{
		int diff = sum - expectedSum;
		int diffRadius = diff / 2;
		for ( int i = -diffRadius; i <= diffRadius; i++)
		{
			(*o_results)[io_radius + i]--;
			sum--;
		}
		if (diff %2 == 0)
		{
			(*o_results)[io_radius]++;
			sum++;
		}
	}

	if (sum < expectedSum)
	{
		int diff = expectedSum - sum;
		int diffRadius = diff / 2;
		for ( int i = -diffRadius; i <= diffRadius; i++)
		{
			(*o_results)[io_radius + i]++;
		}
		if (diff%2 == 0)
		{
			(*o_results)[io_radius]--;
		}
	}
	io_radius = io_radius - firstNonZero;
	delete []fWeights;
}

extern "C" void Horizontal_mmx_fir_line(int radius, int width, int height, short *weightInt, unsigned long *lpPixelBufferTemp, unsigned long *lpPixelBufferLine);
extern "C" void Vertical_mmx_fir_line(int radius, int width, int height, short *weightInt, unsigned long *lpPixelBufferDest, unsigned long *lpPixelBufferLine);
// 这个方法的C++版本请看OneDimentionRender
void OneDimentionRenderMMX(XL_BITMAP_HANDLE hBitmap, const float &i_sigma, int i_radius)
{
	assert(hBitmap);
	XLBitmapInfo bmp;
	XL_GetBitmapInfo(hBitmap, &bmp);

	assert(bmp.ColorType == XLGRAPHIC_CT_ARGB32);

	short *weightBufferInitial = NULL;
	short *weightInt = NULL;
	short *weights = NULL;
	// 对半径做矫正, 因为要右移8位丢失精度, 所以半径大于128的像素是么有意义的
	if (i_radius > 128)
	{
		i_radius = 128;
	}
	if (i_radius > bmp.Width / 2 - 1)
	{
		i_radius = bmp.Width / 2 - 1;
	}
	if (i_radius > bmp.Height / 2 - 1)
	{
		i_radius = bmp.Height / 2 - 1;
	}

	// TODO: 这里改一改, 太难看了. 
	int radius = i_radius;
	GaussianFunctionInteger(i_sigma, radius, &weightBufferInitial, 8);
	if (radius == 0)
	{
		radius = 1;
	}
	weightInt = weightBufferInitial + i_radius - radius;
	weights = weightInt;

	unsigned long *lpPixelBufferLine = NULL;
	unsigned long *lpPixelBufferInitial = (unsigned long*)XL_GetBitmapBuffer(hBitmap, 0, 0);
	unsigned long *lpPixelBufferTempInitial = (unsigned long*)malloc(sizeof(unsigned long) * bmp.Height * bmp.Width);
	int scanLengthInDW = bmp.ScanLineLength / 4;

	unsigned long heightInBytes = bmp.Height * 4;
	unsigned long *lpPixelBufferTemp = lpPixelBufferTempInitial +  bmp.Height * (bmp.Width - 1);
	unsigned long *lpPixelBufferTempEnd = lpPixelBufferTempInitial +  bmp.Height * (bmp.Width - 1);
	
#pragma omp parallel for 
	for (int line = 0; line < bmp.Height; ++line)
	{
		lpPixelBufferLine = lpPixelBufferInitial + scanLengthInDW * line;
		lpPixelBufferTemp = lpPixelBufferTempEnd + line;
		Horizontal_mmx_fir_line(radius,  bmp.Width,  heightInBytes,  weightInt, lpPixelBufferTemp, lpPixelBufferLine);
	}

	unsigned long *lpPixelBufferDest;
	unsigned long *lpPixelBufferDestEnd = lpPixelBufferInitial + scanLengthInDW * (bmp.Height - 1);
	
#pragma omp parallel for 
	for (int column = 0; column < bmp.Width; ++column)
	{
		lpPixelBufferLine = lpPixelBufferTempInitial + column * bmp.Height; // 线头
		lpPixelBufferDest = lpPixelBufferDestEnd + column; // 线尾
		Vertical_mmx_fir_line(radius, bmp.ScanLineLength, bmp.Height, weightInt, lpPixelBufferDest, lpPixelBufferLine);
	}
	delete []weightBufferInitial;
	free(lpPixelBufferTempInitial);
}

#ifdef DEBUG
void GaussianFunction(float sigma, int r, float ** results);
void GaussianFunction2(float sigma, int r, float **results);
/*
这个方法在正式版本中没有用到, SSE的性能并不好

但是, 我在考虑用它来实现模糊系数比较小, 图片也比较小(例如字体)的模糊. 
因为, 丢失精度的, 半径为1的高斯核会变成(0, 256, 0)这样, 跟没做模糊没区别. 
*/
void OneDimentionRenderSSE(XL_BITMAP_HANDLE hBitmap, float m_sigma, int m_radius)
{
	assert(hBitmap);
	XLBitmapInfo bmp;
	XL_GetBitmapInfo(hBitmap, &bmp);

	assert(bmp.ColorType == XLGRAPHIC_CT_ARGB32);

	int diameter = m_radius * 2 + 1;
	float *weight = NULL;
	GaussianFunction(m_sigma, m_radius, &weight);

	unsigned long *lpPixelBufferLine = NULL;
	unsigned long *lpPixelBufferInitial = (unsigned long*)XL_GetBitmapBuffer(hBitmap, 0, 0);
	unsigned long *lpPixelBufferTempInitial = (unsigned long*)malloc(sizeof(unsigned long)*bmp.Height*bmp.Width);

	int lo = 0;
	int hi =  bmp.Width - 1;
	int scanLineLengthInPixel = bmp.ScanLineLength / 4;

	for (int line = 0; line < bmp.Height; ++line)
	{
		lpPixelBufferLine = lpPixelBufferInitial + scanLineLengthInPixel*line;
		unsigned long *lpPixelBufferTemp = lpPixelBufferTempInitial + line;
		for (int col = 0; col < bmp.Width; ++col)
		{
			_asm
			{
				mov edx, diameter;
				mov ecx, weight;让ecx指向weight的首地址, 每次循环加4byte(1个float那么长)指向weight[m_radius+j];
start_loop_h:
				; 开始循环体
				mov ebx, m_radius;
				sub ebx, edx;
				add ebx, 1; // 现在bx = j
				add ebx, col; // now bx = col + j

				cmp ebx, lo;
				jge gt_than_low_h;
				mov ebx, lo;
gt_than_low_h:
				cmp ebx, hi;
				jle ls_than_high_h;
				mov ebx, hi;
ls_than_high_h:
				;矫正完了, ebx存放着正确的索引 即col+j被矫正在0-width之内

				; 行首地址放在eax, offset放在ebx, offset*dword长加在行首eax, 对eax代表的地址取值,
				; 看起来很罗嗦但是不知道为什么 mov lpPixelBufferLine, [dword eax + indx]不工作
				; Todo: 效率?
				mov eax, lpPixelBufferLine; 
				imul ebx, 4;
				add eax, ebx; 

				; 把pixelbuffer的四个8bit integer扩展成4个16bit integer, 放在64bit寄存器mmx0上 
				pmovzxbd xmm0, [eax];
				cvtdq2ps xmm0, xmm0; eax用完了

				movd xmm1, [ecx]; 现在xmm1的低四位存放weight[m_radius+j]了
				shufps xmm1, xmm1, 0x00;
				mulps xmm0, xmm1;

				addps xmm2, xmm0;

				; 结束循环体
				add ecx, 4;
				dec edx;
				jnz start_loop_h;

				mov esi, lpPixelBufferTemp; esi now points at destination

				; 想要把xmm2的每一个float取整截成byte,拼成一个long
				cvtps2dq xmm2, xmm2;
				packssdw xmm2, xmm2;
				pextrw [esi], xmm2, 0;
				pextrw [esi+1], xmm2, 1;
				pextrw [esi+2], xmm2, 2;
				;pextrw [esi+3], xmm2, 3;
				mov [esi+3], 0xfe;

				emms;
			}
			lpPixelBufferTemp += bmp.Height;
		}
	}

	lo = 0;
	hi = bmp.Height - 1;
	for (int column = 0; column < bmp.Width; ++column)
	{
		lpPixelBufferLine = lpPixelBufferTempInitial + column * bmp.Height;
		for (int row = 0; row < bmp.Height; ++row)
		{
			unsigned long *lpPixelBuffer = lpPixelBufferInitial + scanLineLengthInPixel * row + column;
			_asm
			{
				mov edx, m_radius;
				imul edx, 2;
				add edx, 1;
				mov ecx, weight;
start_loop_v:
				mov ebx, m_radius;
				sub ebx, edx;
				add ebx, 1;
				add ebx, row;

				cmp ebx, lo;
				jge gt_than_low_v;
				mov ebx, lo;
gt_than_low_v:
				cmp ebx, hi;
				jle ls_than_high_v;
				mov ebx, hi;
ls_than_high_v:
				;矫正结束, ebx存放正确索引

					mov eax, lpPixelBufferLine;
				imul ebx, 4;
				add eax, ebx;

				pmovzxbd xmm0, [eax];
				cvtdq2ps xmm0, xmm0; 整形变浮点

				movd xmm1, [ecx];
				shufps xmm1, xmm1, 0x00;
				mulps xmm0, xmm1;
				addps xmm2, xmm0;

				add ecx, 4;
				dec edx;
				jnz start_loop_v;

				; 想要把xmm2的每一个float取整截成byte,拼成一个long
					cvtps2dq xmm2, xmm2;
				packssdw xmm2, xmm2;
				mov esi, lpPixelBuffer;
				pextrw [esi], xmm2, 0;
				pextrw [esi+1], xmm2, 1;
				pextrw [esi+2], xmm2, 2;
				;pextrw [esi+3], xmm2, 3;
				mov [esi+3], 0xfe;这一位是alpha
					emms;
			}
		}
	}
	free(lpPixelBufferTempInitial);
}

/*
FIR高斯模糊的两个维度叠加模糊的c++实现;
先做水平方向的模糊, 原图做输入, 结果写入原图buffer;
再做垂直方向的模糊, 水平模糊过的原图做输入, 结果写入原图buffer
没有进行转置, 也没有另外开辟图片一样大小的内存空间来放中间结果
这个实现只是为了验证两个维度叠加的高斯模糊和两个维度同时计算的高斯模糊是一样的, 不会最终使用, 因为太慢了!
*/
void OneDimentionRender(XL_BITMAP_HANDLE hBitmap, float m_sigma, int m_radius)
{
	assert(hBitmap);
	XLBitmapInfo bmp;
	XL_GetBitmapInfo(hBitmap, &bmp);

	assert(bmp.ColorType == XLGRAPHIC_CT_ARGB32);

	int diameter = m_radius * 2 + 1;
	float *weight = NULL;
	GaussianFunction(m_sigma, m_radius, &weight);

	unsigned long *lpPixelBufferLine;
	unsigned long *lpPixelBufferInitial = (unsigned long*)XL_GetBitmapBuffer(hBitmap, 0, 0);
	int scanLineLengthInPixel = bmp.ScanLineLength / 4;
	int bmpWidthInBytes = bmp.Width * 4;

	lpPixelBufferLine = new unsigned long [bmp.Width];
	for (int line = 0; line < bmp.Height; ++line)
	{
		memcpy(lpPixelBufferLine, lpPixelBufferInitial + scanLineLengthInPixel * line, bmpWidthInBytes);
		for (int col = 0; col < bmp.Width; ++col)
		{
			unsigned long *lpPixelBuffer = lpPixelBufferInitial + scanLineLengthInPixel * line + col;
			float redSum = 0;
			float greenSum = 0;
			float blueSum = 0;
			float alphaSum = 0;
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
				unsigned int alpha = XLCOLOR_BGRA_A(pixelBuffer);
				unsigned int green = XLCOLOR_BGRA_G(pixelBuffer);
				unsigned int red = XLCOLOR_BGRA_R(pixelBuffer);
				unsigned int blue = XLCOLOR_BGRA_B(pixelBuffer);
				redSum += red * weight[j + m_radius];
				greenSum += green * weight[j + m_radius];
				blueSum += blue * weight[j + m_radius];
				alphaSum += alpha * weight[j + m_radius];
			}
			unsigned int alpha = alphaSum;
			unsigned int green = greenSum;
			unsigned int red = redSum;
			unsigned int blue = blueSum;
			*lpPixelBuffer = XLCOLOR_BGRA(blue, green, red, alpha);
		}
	}
	delete []lpPixelBufferLine;
	lpPixelBufferLine = new unsigned long [bmp.Height];
	for (int column = 0; column < bmp.Width; ++column)
	{
		for (int row = 0; row < m_radius; ++row)
		{
			lpPixelBufferLine[row] = *(lpPixelBufferInitial + scanLineLengthInPixel * row + column);
		}
		for (int row = 0; row < bmp.Height; ++row)
		{
			if (row + m_radius < bmp.Height)
			{
				lpPixelBufferLine[row + m_radius] = *(lpPixelBufferInitial + scanLineLengthInPixel * (row + m_radius) + column);
			}
			unsigned long *lpPixelBuffer = lpPixelBufferInitial + scanLineLengthInPixel * row + column;
			float redSum = 0;
			float greenSum = 0;
			float blueSum = 0;
			float alphaSum = 0;
			for (int j = -m_radius; j <= m_radius; j++)
			{
				unsigned long pixelBuffer;
				if (row + j < 0)
				{
					pixelBuffer = lpPixelBufferLine[0];
				}
				else if (row + j >= bmp.Height)
				{
					pixelBuffer = lpPixelBufferLine[bmp.Height - 1];
				}
				else
				{
					pixelBuffer = lpPixelBufferLine[row + j];
				}
				unsigned int alpha = XLCOLOR_BGRA_A(pixelBuffer);
				unsigned int green = XLCOLOR_BGRA_G(pixelBuffer);
				unsigned int red = XLCOLOR_BGRA_R(pixelBuffer);
				unsigned int blue = XLCOLOR_BGRA_B(pixelBuffer);
				redSum += red * weight[j + m_radius];
				greenSum += green * weight[j + m_radius];
				blueSum += blue * weight[j + m_radius];
				alphaSum += alpha * weight[j + m_radius];
			}
			unsigned int alpha = alphaSum;
			unsigned int green = greenSum;
			unsigned int red = redSum;
			unsigned int blue = blueSum;
			*lpPixelBuffer = XLCOLOR_BGRA(blue, green, red, alpha);
		}
	}
	delete []lpPixelBufferLine;
}

/*
memcpy bmp.Height 次调用在bmp.Width * 4长度上
XL_GetBitmapBuffer 1次
*/
void TwoDimentionRender(XL_BITMAP_HANDLE hBitmap, float m_sigma, int m_radius)
{
	assert(hBitmap);
	XLBitmapInfo bmp;
	XL_GetBitmapInfo(hBitmap, &bmp);

	assert(bmp.ColorType == XLGRAPHIC_CT_ARGB32);

	int diameter = m_radius * 2 + 1;
	float *weight = NULL;
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
		unsigned long *lpPixelBufferBmpLine = lpPixelBufferInitial + lineWidth / 4 * line;
		// 把m_radius行之前的buffer释放掉, 因为后面都用不到了
		if ( line >= m_radius + 1)
		{
			delete []lpPixelBufferLines[line - m_radius - 1];
		}
		// 提前加载m_radius行之后的line, 因为要用到
		if (line + m_radius < bmp.Height)
		{
			lpPixelBufferLines[line + m_radius] = new unsigned long[bmp.Width];
			unsigned long *lpPixelBufferBmpLinePlusRadius = lpPixelBufferInitial + lineWidth / 4 * (line + m_radius);
			memcpy(lpPixelBufferLines[line + m_radius], lpPixelBufferBmpLinePlusRadius, bmp.Width * 4);
		}

		for (int col = 0; col < bmp.Width; ++col)
		{
			unsigned long *lpPixelBuffer = lpPixelBufferBmpLine + col;
			float redSum = 0;
			float greenSum = 0;
			float blueSum = 0;
			float alphaSum = 0;
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

					unsigned int alpha = XLCOLOR_BGRA_A(pixelBuffer);
					unsigned int green = XLCOLOR_BGRA_G(pixelBuffer);
					unsigned int red = XLCOLOR_BGRA_R(pixelBuffer);
					unsigned int blue = XLCOLOR_BGRA_B(pixelBuffer);
					redSum += red * weight[(i + m_radius) * diameter + (j + m_radius)];
					greenSum += green * weight[(i + m_radius) * diameter + (j + m_radius)];
					blueSum += blue * weight[(i + m_radius) * diameter + (j + m_radius)];
					alphaSum += alpha * weight[(i + m_radius) * diameter + (j + m_radius)];
				}
			}
			unsigned int alpha = alphaSum;
			unsigned int green = greenSum;
			unsigned int red = redSum;
			unsigned int blue = blueSum;
			*lpPixelBuffer = XLCOLOR_BGRA(blue, green, red, alpha);
		}
	}
	for (unsigned long bufferLine = bmp.Height - m_radius - 1; bufferLine < bmp.Height; bufferLine++)
	{
		delete []lpPixelBufferLines[bufferLine];
	}
	delete []lpPixelBufferLines;
}
void GaussianFunction(float sigma, int r, float ** results)
{
	*results =new float[r * 2 + 1];
	float sum = 0;
	float factor = 1.0 / sigma / sqrt(2 * pi);
	for ( int i=0; i < r + 1; i++)
	{
		(*results)[i] = exp(0 - (i - r) * (i - r) / (2 * sigma * sigma)) * factor;
		sum += (*results)[i];
	}
	for (int i = r + 1; i < r * 2 + 1; i++)
	{
		(*results)[i] = (*results)[r * 2 - i];
		sum += (*results)[i];
	}
	for (int i = 0; i < r * 2 + 1; i++)
	{
		(*results)[i] /= sum;
	}
}

void GaussianFunction2(float sigma, int r, float **results)
{
	int D = r * 2 + 1;
	*results = new float[D * D];
	float sum = 0;
	float factor = 1.0 / 2 / pi / sigma / sigma;
	for ( int i = 0; i < r + 1; i++)
	{
		for (int j = 0; j < r + 1; j++)
		{
			(*results)[i * D + j] = exp(0 - ((i - r) * (i - r) + (j - r) * (j - r)) / (2 * sigma * sigma)) * factor; 
			sum += (*results)[i * D + j];
		}
	}
	for (int i = r + 1; i < D; i++)
	{
		for (int j= 0; j < r + 1; j++)
		{
			(*results)[i * D + j] = (*results)[(r * 2 - i) * D + j];
			sum += (*results)[i * D + j];
		}
	}
	for (int i = 0; i < r + 1; i++)
	{
		for (int j = r + 1; j < D; j++)
		{
			(*results)[i * D + j] = (*results)[i * D + (r * 2 - j)];
			sum += (*results)[i * D + j];
		}
	}
	for (int i = r + 1; i < D; i++)
	{
		for (int j = r + 1; j < D; j++)
		{
			(*results)[i * D + j] = (*results)[(r * 2 - i) * D + (r * 2 - j)];
			sum += (*results)[i * D + j];
		}
	}
}

#endif