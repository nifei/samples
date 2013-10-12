#if 0
			for (__int16 j = -m_radius; j <= m_radius; j++)
			{
				_asm{
					mov bx, col;
					add bx, j;
					; 现在 bx = col + j
					; 矫正bx在范围内
					mov ax, lo;
					mov cx, hi;
					cmp bx, ax;
					jge greater_than_low;
less_than_low:
					mov bx, ax;
greater_than_low:
					cmp bx, cx;
					jle less_than_high;
greater_than_high:
					mov bx, cx;
less_than_high:
					;没什么好做的, 矫正完了, bx存放着正确的索引
					
					; 行首地址放在eax, offset放在ebx, offset*dword长加在行首eax, 对eax代表的地址取值,
					; 看起来很罗嗦但是不知道为什么 mov pixelBufer, [dword eax + indx]不工作
					; Todo: 效率?
					mov eax, lpPixelBufferLine; 
					movzx ebx, bx; bx用完了
					imul ebx, 4;
					add eax, ebx;
					mov ecx, [eax]; 

					;把pixelBuffer的四个8bit integer扩展成4个32bit integer, 放在128bit寄存器xmm0上
					pmovzxbd xmm0, [eax];
					cvtdq2ps xmm0, xmm0; eax, ebx, ecx用完了

					; 先把weighting = weight[m_radius+j]读出来
					mov bx, m_radius;
					add bx, j;
					movzx ebx, bx;
					mov eax, weight;
					imul ebx, 4;
					add eax, ebx;
					; 此时eax指向weight[m_radius+j], 为了把eax表示的地址存放的float放在xmm1上花费
					; 了两个多小时用什么指令都不对
					; 然后发现是因为weight的类型为double T.T
					; 教训就是注意实际在内存中, 寄存器上存放的数据的实际类型, eax指向的地址是正确的, 但是要两倍
					; 的长度才能拿到正确的值, 而我一直当float的长度来move, 当然不对
					movd xmm1, [eax]; 现在xmm1的低四位存放weight[m_radius+j]了
					shufps xmm1, xmm1, 0x00;
					mulps xmm0, xmm1;

					movupd xmm2, pixelSum;
					addps xmm2, xmm0;
					movupd pixelSum, xmm2;
					emms;
				}
			}
#endif


删掉
		for (__int32 col = 0; col < bmp.Width; ++col)
		{
			float pixelSum[4] = {0,0,0,0};
			__int32 lo = 0;
			__int32 hi = bmp.Width - 1;
			char a,r,g,b;

			// 这一段asm用到的内存变量有:
			// lo - 这根线的索引最小值
			// hi - 这根线的索引最大值
			// m_radius, 高斯核半径, __int32
			// weight, 高斯核矩阵
			// col, 我们正在计算这根线上第col个像素
			// lpPixelBufferLine - 这根线的首地址
			// pixelSum - 要在上面累积颜色分量的向量
			_asm{
				mov edx, m_radius;
				imul edx, 2;
				add edx, 1;
				mov ecx, 0; 让ecx指向weight的首地址, 每次循环加4byte(1个float那么长)指向weight[m_radius+j];
				add ecx, weight;
				movupd xmm2, pixelSum;
start_loop:
				; 开始循环体
				mov ebx, m_radius;
				sub ebx, edx;
				add ebx, 1; // 现在bx = j
				add ebx, col; // now bx = col + j

				cmp ebx, lo;
				jge gt_than_low;
ls_than_low:
				mov ebx, lo;
gt_than_low:
				cmp ebx, hi;
				jle ls_than_high;
gs_than_high:
				mov ebx, hi;
ls_than_high:
				;矫正完了, ebx存放着正确的索引 即col+j被矫正在0-width之内
				
				; 行首地址放在eax, offset放在ebx, offset*dword长加在行首eax, 对eax代表的地址取值,
				; 看起来很罗嗦但是不知道为什么 mov lpPixelBufferLine, [dword eax + indx]不工作
				; Todo: 效率?
				mov eax, lpPixelBufferLine; 
				imul ebx, 4;
				add eax, ebx; 

				;把pixelBuffer的四个8bit integer扩展成4个32bit integer, 放在128bit寄存器xmm0上
				pmovzxbd xmm0, [eax];
				cvtdq2ps xmm0, xmm0; eax用完了

				; 先把weighting = weight[m_radius+j]读出来
				; 此时ecx指向weight[m_radius+j], 为了把ecx表示的地址存放的float放在xmm1上花费
				; 了两个多小时用什么指令都不对
				; 然后发现是因为weight的类型为double T.T
				; 教训就是注意实际在内存中, 寄存器上存放的数据的实际类型, ecx指向的地址是正确的, 但是要两倍
				; 的长度才能拿到正确的值, 而我一直当float的长度来move, 当然不对
				movd xmm1, [ecx]; 现在xmm1的低四位存放weight[m_radius+j]了
				shufps xmm1, xmm1, 0x00;
				mulps xmm0, xmm1;

				addps xmm2, xmm0;

				; 结束循环体
				add ecx, 4;
				dec edx;
				jnz start_loop;
				movupd pixelSum, xmm2;
				; 想要把xmm2的每一个float取整截成byte,拼成一个long
				cvtps2dq xmm2, xmm2;
				packssdw xmm2, xmm2;
				pextrw b, xmm2, 0;
				pextrw g, xmm2, 1;
				pextrw r, xmm2, 2;
				pextrw a, xmm2, 3;
				emms;
			}
			*lpPixelBufferTemp = XLCOLOR_BGRA(b, g, r, a);
			lpPixelBufferTemp += bmp.Height;
		}
		delete []lpPixelBufferLine;
	}