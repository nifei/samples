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
