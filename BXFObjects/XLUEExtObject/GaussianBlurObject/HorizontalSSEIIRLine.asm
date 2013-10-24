; Stretch SSE main loop implement

;void horizontal_sse_iir_line(int destHeight,int dstWidth,int destScanLine,DWORD* pDstLine,
			;const unsigned char* pSrcBuffer,int srcScanline,unsigned long xrIntFloat16,
			;unsigned long yrIntFloat16)
			

BITS 32

SECTION .text			
global _horizontal_sse_iir_line

_horizontal_sse_iir_line:
	push ebp
	mov ebp, esp
	
	%define dst_height [dword ebp + 8]
	%define dst_width [dword ebp + 0x0c]
	%define dst_scanline_len [dword ebp + 0x10]
	%define dst_buf [dword ebp + 0x14]
	%define src_buf [dword ebp + 0x18]
	%define src_scanline_len [dword ebp + 0x1c]
	%define xr_int_float_16 [dword ebp + 0x20]
	%define yr_int_float_16 [dword ebp + 0x24]
	
	sub esp, 0x0c
	%define src_y_16 [dword ebp - 4]
	%define src_line [dword ebp - 8]
	%define dst_line [dword ebp - 0x0c]
	
	sub esp, 0x18
	push edi
	push esi
	push eax
	push ebx
	push ecx
	push edx
	
	
	mov eax, dst_buf
	mov dst_line, eax
	mov eax, 0
	mov src_y_16, eax
	
	;for (unsigned long y = 0; y < dest->GetHeight(); ++y)
	;{
	
.loop_y_begin:

		;DWORD* pSrcLine = ((DWORD*)(Bitmap_GetBuffer(src,0, 0) + src->GetScanLineLength() * (srcy16 >> 16)))
		mov eax, src_y_16
		shr eax, 16
		mov ebx, src_scanline_len
		mul ebx
		add eax, src_buf
		mov src_line, eax
			
				mov esi, src_line
				mov edi, dst_line
				mov edx, xr_int_float_16
				mov ecx, dst_width
				xor eax, eax
				
				;and ecx, (not 3)
				and ecx, ~3
				test ecx, ecx
				jle .loop_end
				lea edi, [edi+ecx*4]
				neg ecx
.write_loop:
				mov ebx, eax
				shr eax, 16
				movd mm0, [esi+eax*4]
				mov eax, ebx
				lea ebx, [eax+edx]
				shr ebx, 16
				punpckldq mm0, [esi+ebx*4]
				lea eax, [eax+edx*2]
				movntq qword [edi+ecx*4], mm0
				mov ebx, eax
				shr eax, 16
				movd mm1, [esi+eax*4]
				mov eax, ebx
				lea ebx, [eax+edx]    
				shr ebx, 16
				punpckldq mm1, [esi+ebx*4]
				lea eax, [eax+edx*2]
				movntq qword [edi+ecx*4+8], mm1
				add ecx, 4
				jnz .write_loop
.loop_end:
				mov ebx, eax
				mov ecx, dst_width
				and ecx, 3
				test ecx, ecx
				jle .loop_y_end
				lea edi, [edi+ecx*4]
				neg ecx
.border:
				mov eax, ebx
				shr eax, 16
				mov eax, [esi+eax*4]
				mov [edi+ecx*4], eax
				add ebx, edx
				inc ecx
				jnz .border
.loop_y_end:
				sfence
				emms
		
		;srcy16 += yrIntFloat16
		mov eax, src_y_16
		add eax, yr_int_float_16
		mov src_y_16, eax
		;pDstLine = (DWORD*)((unsigned char*)pDstLine + dest->GetScanLineLength())
		mov eax, dst_line
		add eax, dst_scanline_len
		mov dst_line, eax
		
		mov ecx, dst_height
		dec ecx
		mov dst_height, ecx
		jnz .loop_y_begin
	;}
	
	pop edx
	pop ecx
	pop ebx
	pop eax
	pop esi
	pop edi
	
	leave 
	ret 