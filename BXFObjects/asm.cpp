#if 0
			for (__int16 j = -m_radius; j <= m_radius; j++)
			{
				_asm{
					mov bx, col;
					add bx, j;
					; ���� bx = col + j
					; ����bx�ڷ�Χ��
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
					;ûʲô������, ��������, bx�������ȷ������
					
					; ���׵�ַ����eax, offset����ebx, offset*dword����������eax, ��eax����ĵ�ַȡֵ,
					; �����������µ��ǲ�֪��Ϊʲô mov pixelBufer, [dword eax + indx]������
					; Todo: Ч��?
					mov eax, lpPixelBufferLine; 
					movzx ebx, bx; bx������
					imul ebx, 4;
					add eax, ebx;
					mov ecx, [eax]; 

					;��pixelBuffer���ĸ�8bit integer��չ��4��32bit integer, ����128bit�Ĵ���xmm0��
					pmovzxbd xmm0, [eax];
					cvtdq2ps xmm0, xmm0; eax, ebx, ecx������

					; �Ȱ�weighting = weight[m_radius+j]������
					mov bx, m_radius;
					add bx, j;
					movzx ebx, bx;
					mov eax, weight;
					imul ebx, 4;
					add eax, ebx;
					; ��ʱeaxָ��weight[m_radius+j], Ϊ�˰�eax��ʾ�ĵ�ַ��ŵ�float����xmm1�ϻ���
					; ��������Сʱ��ʲôָ�����
					; Ȼ��������Ϊweight������Ϊdouble T.T
					; ��ѵ����ע��ʵ�����ڴ���, �Ĵ����ϴ�ŵ����ݵ�ʵ������, eaxָ��ĵ�ַ����ȷ��, ����Ҫ����
					; �ĳ��Ȳ����õ���ȷ��ֵ, ����һֱ��float�ĳ�����move, ��Ȼ����
					movd xmm1, [eax]; ����xmm1�ĵ���λ���weight[m_radius+j]��
					shufps xmm1, xmm1, 0x00;
					mulps xmm0, xmm1;

					movupd xmm2, pixelSum;
					addps xmm2, xmm0;
					movupd pixelSum, xmm2;
					emms;
				}
			}
#endif


ɾ��
		for (__int32 col = 0; col < bmp.Width; ++col)
		{
			float pixelSum[4] = {0,0,0,0};
			__int32 lo = 0;
			__int32 hi = bmp.Width - 1;
			char a,r,g,b;

			// ��һ��asm�õ����ڴ������:
			// lo - ����ߵ�������Сֵ
			// hi - ����ߵ��������ֵ
			// m_radius, ��˹�˰뾶, __int32
			// weight, ��˹�˾���
			// col, �������ڼ���������ϵ�col������
			// lpPixelBufferLine - ����ߵ��׵�ַ
			// pixelSum - Ҫ�������ۻ���ɫ����������
			_asm{
				mov edx, m_radius;
				imul edx, 2;
				add edx, 1;
				mov ecx, 0; ��ecxָ��weight���׵�ַ, ÿ��ѭ����4byte(1��float��ô��)ָ��weight[m_radius+j];
				add ecx, weight;
				movupd xmm2, pixelSum;
start_loop:
				; ��ʼѭ����
				mov ebx, m_radius;
				sub ebx, edx;
				add ebx, 1; // ����bx = j
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
				;��������, ebx�������ȷ������ ��col+j��������0-width֮��
				
				; ���׵�ַ����eax, offset����ebx, offset*dword����������eax, ��eax����ĵ�ַȡֵ,
				; �����������µ��ǲ�֪��Ϊʲô mov lpPixelBufferLine, [dword eax + indx]������
				; Todo: Ч��?
				mov eax, lpPixelBufferLine; 
				imul ebx, 4;
				add eax, ebx; 

				;��pixelBuffer���ĸ�8bit integer��չ��4��32bit integer, ����128bit�Ĵ���xmm0��
				pmovzxbd xmm0, [eax];
				cvtdq2ps xmm0, xmm0; eax������

				; �Ȱ�weighting = weight[m_radius+j]������
				; ��ʱecxָ��weight[m_radius+j], Ϊ�˰�ecx��ʾ�ĵ�ַ��ŵ�float����xmm1�ϻ���
				; ��������Сʱ��ʲôָ�����
				; Ȼ��������Ϊweight������Ϊdouble T.T
				; ��ѵ����ע��ʵ�����ڴ���, �Ĵ����ϴ�ŵ����ݵ�ʵ������, ecxָ��ĵ�ַ����ȷ��, ����Ҫ����
				; �ĳ��Ȳ����õ���ȷ��ֵ, ����һֱ��float�ĳ�����move, ��Ȼ����
				movd xmm1, [ecx]; ����xmm1�ĵ���λ���weight[m_radius+j]��
				shufps xmm1, xmm1, 0x00;
				mulps xmm0, xmm1;

				addps xmm2, xmm0;

				; ����ѭ����
				add ecx, 4;
				dec edx;
				jnz start_loop;
				movupd pixelSum, xmm2;
				; ��Ҫ��xmm2��ÿһ��floatȡ���س�byte,ƴ��һ��long
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