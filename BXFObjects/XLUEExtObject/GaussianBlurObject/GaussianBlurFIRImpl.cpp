#include "stdafx.h"
#include "GaussianBlurDelegate.h"
#include <cmath>
#include <omp.h>

void GaussianFunction(double sigma, int r, float ** results);
void GaussianFunction2(double sigma, int r, double **results);

void OneDimentionRenderSSE(XL_BITMAP_HANDLE hBitmap, double m_sigma, __int32 m_radius)
{
	assert(hBitmap);
	XLBitmapInfo bmp;
	XL_GetBitmapInfo(hBitmap, &bmp);

	assert(bmp.ColorType == XLGRAPHIC_CT_ARGB32);

	int diameter = m_radius * 2 + 1;
	float *weight;
	GaussianFunction(m_sigma, m_radius, &weight);

	unsigned long *lpPixelBufferLine;
	unsigned long *lpPixelBufferInitial = (unsigned long*)XL_GetBitmapBuffer(hBitmap, 0, 0);
	unsigned long *lpPixelBufferTempInitial = (unsigned long*)malloc(sizeof(unsigned long)*bmp.Height*bmp.Width);

	__int32 lo = 0;
	__int32 hi = 0;
	_asm{
		jmp done;
correct_index:
		; index in ebx
		; lo
		; hi
		cmp ebx, lo;
		jge gt_than_low_case;
ls_than_low_case:
		mov ebx, lo;
gt_than_low_case:
		cmp ebx, hi;
		jle ls_than_high_case;
gs_than_high_case:
		mov ebx, hi;
ls_than_high_case:
		ret;
done:
	}
	for (int line = bmp.Height - 1; line >= 0; --line)
	{
		lpPixelBufferLine = lpPixelBufferInitial + bmp.ScanLineLength/4*line;

		lo = 0;
		hi = bmp.Width - 1;
		_asm {
			// ebx 记循环
			// esi 记lpPixelBufferTemp
			mov ebx,bmp.Width;
			mov esi, lpPixelBufferTempInitial;
			add esi, line;
			add esi, line;
			add esi, line;
			add esi, line;
			mov edx, diameter;
start_loop_h_level_1:
				push edx;
				mov ecx, weight;让ecx指向weight的首地址, 每次循环加4byte(1个float那么长)指向weight[m_radius+j];
start_loop_h_level_0:
				push ebx
				add ebx, m_radius
				sub ebx, edx
				call correct_index;

				mov eax, lpPixelBufferLine ;为什么不能用递增/减: 边界溢出的像素要取边界点, 非线性递增/减关系; 但是中间大部分都是递增/减的.
				imul ebx, 4;
				add eax, ebx; 
				pmovzxbd xmm0, [eax];
				cvtdq2ps xmm0, xmm0; eax用完了

				movd xmm1, [ecx]; 现在xmm1的低四位存放weight[m_radius+j]了
				shufps xmm1, xmm1, 0x00;
				mulps xmm0, xmm1;

				addps xmm2, xmm0;

				; 结束循环体
				pop ebx
				add ecx, 4;
				dec edx;
				jnz start_loop_h_level_0;

				; 想要把xmm2的每一个float取整截成byte,拼成一个long
				cvtps2dq xmm2, xmm2;
				packssdw xmm2, xmm2;
				pextrw [esi], xmm2, 0;
				pextrw [esi+1], xmm2, 1;
				pextrw [esi+2], xmm2, 2;
				;pextrw [esi+3], xmm2, 3;
				mov [esi+3], 0xfe;

				pop edx;
end_loop_h_level_1:
			add esi, bmp.Height;
			add esi, bmp.Height;
			add esi, bmp.Height;
			add esi, bmp.Height;
			dec ebx;
			jnz start_loop_h_level_1;
			emms;
		}
	}
	for (int column = bmp.Width - 1; column >= 0; --column)
	{
		lpPixelBufferLine = lpPixelBufferTempInitial + (bmp.Width-1-column) * bmp.Height;
		lo = 0;
		hi = bmp.Height - 1;
		_asm{
			// ebx记这一层的循环
			// esi记要写的位置
			mov ebx, bmp.Height;
			mov edx, diameter;
			mov ecx, weight;
			mov esi, bmp.Height;
			sub esi, 1;
			imul esi, bmp.ScanLineLength;
			add esi, lpPixelBufferInitial;
			add esi, column;
			add esi, column;
			add esi, column;
			add esi, column;
			// expected: esi pointing at end of column-th column, and each time esi-= bmp.ScanLineLength to point at the above pixel 
start_loop_v_level_1:
			push ecx;// ecx 停留在weight起点
			push edx;// 高斯核循环次数, 记下来, 循环过一次高斯核之后恢复
			mov eax, lpPixelBufferLine;
start_loop_v_level_0:
				push ebx;// ebx 停止表示循环次数
				push eax;
				
				add ebx, m_radius;
				sub ebx, edx;
				call correct_index;//calibri ebx

				add eax, ebx;
				add eax, ebx;
				add eax, ebx;
				add eax, ebx;

				pmovzxbd xmm0, [eax];
				cvtdq2ps xmm0, xmm0; 整形变浮点

				movd xmm1, [ecx];
				shufps xmm1, xmm1, 0x00;
				mulps xmm0, xmm1;
				addps xmm2, xmm0;
end_loop_v_level_0:
				add ecx, 4;//在weight上前进一步
				pop eax;
				pop ebx;// ebx重新开始表示循环次数
				dec edx;
				jnz start_loop_v_level_0;

			; 想要把xmm2的每一个float取整截成byte,拼成一个long
			cvtps2dq xmm2, xmm2;
			packssdw xmm2, xmm2;

			pextrw [esi], xmm2, 0;
			pextrw [esi+1], xmm2, 1;
			pextrw [esi+2], xmm2, 2;
			;pextrw [esi+3], xmm2, 3;
			mov [esi+3], 0xfe;这一位是alpha
			;add esi, bmp.ScanLineLength;
			sub esi, bmp.ScanLineLength;

			pop edx; // edx回到2*m_radius+1
			pop ecx; // ecx回到weight起点

			dec ebx;
			jnz start_loop_v_level_1;
			emms;
		}
	}
	free(lpPixelBufferTempInitial);
}

void OneDimentionRender(XL_BITMAP_HANDLE hBitmap, double m_sigma, int m_radius)
{
	assert(hBitmap);
	XLBitmapInfo bmp;
	XL_GetBitmapInfo(hBitmap, &bmp);

	assert(bmp.ColorType == XLGRAPHIC_CT_ARGB32);

	int diameter = m_radius * 2 + 1;
	float *weight;
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
				unsigned int alpha = XLCOLOR_BGRA_A(pixelBuffer);
				unsigned int green = XLCOLOR_BGRA_G(pixelBuffer);
				unsigned int red = XLCOLOR_BGRA_R(pixelBuffer);
				unsigned int blue = XLCOLOR_BGRA_B(pixelBuffer);
				redSum += red * weight[j+m_radius];
				greenSum += green * weight[j+m_radius];
				blueSum += blue * weight[j+m_radius];
				alphaSum += alpha * weight[j+m_radius];
			}
			unsigned int alpha = alphaSum;
			unsigned int green = greenSum;
			unsigned int red = redSum;
			unsigned int blue = blueSum;
			*lpPixelBuffer = XLCOLOR_BGRA(blue, green, red, alpha);
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
				unsigned int alpha = XLCOLOR_BGRA_A(pixelBuffer);
				unsigned int green = XLCOLOR_BGRA_G(pixelBuffer);
				unsigned int red = XLCOLOR_BGRA_R(pixelBuffer);
				unsigned int blue = XLCOLOR_BGRA_B(pixelBuffer);
				redSum += red * weight[j+m_radius];
				greenSum += green * weight[j+m_radius];
				blueSum += blue * weight[j+m_radius];
				alphaSum += alpha * weight[j+m_radius];
			}
			unsigned int alpha = alphaSum;
			unsigned int green = greenSum;
			unsigned int red = redSum;
			unsigned int blue = blueSum;
			*lpPixelBuffer = XLCOLOR_BGRA(blue, green, red, alpha);
		}
		delete []lpPixelBufferLine;
	}
}

/*
memcpy bmp.Height 次调用在bmp.Width * 4长度上
XL_GetBitmapBuffer 1次
*/
void TwoDimentionRender(XL_BITMAP_HANDLE hBitmap, double m_sigma, int m_radius)
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

					unsigned int alpha = XLCOLOR_BGRA_A(pixelBuffer);
					unsigned int green = XLCOLOR_BGRA_G(pixelBuffer);
					unsigned int red = XLCOLOR_BGRA_R(pixelBuffer);
					unsigned int blue = XLCOLOR_BGRA_B(pixelBuffer);
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
			*lpPixelBuffer = XLCOLOR_BGRA(blue, green, red, alpha);
		}
	}
	for (unsigned long bufferLine = bmp.Height - m_radius - 1; bufferLine < bmp.Height; bufferLine++)
	{
			delete []lpPixelBufferLines[bufferLine];
	}
	delete []lpPixelBufferLines;
}

const double pi = 3.14159265358979323846;
void GaussianFunction(double sigma, int r, float ** results)
{
	*results =new float[r*2+1];
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
	for (int i = 0; i < r*2+1; i++)
	{
		(*results)[i] /= sum;
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
}
