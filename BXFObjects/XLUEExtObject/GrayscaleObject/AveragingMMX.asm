; /********************************************************************
; /* Copyright (c) 2013 The BOLT UIEngine. All rights reserved.
; /* Use of this source code is governed by a BOLT license that can be
; /* found in the LICENSE file.
; ********************************************************************/ 
;void Horizontal_mmx_fir_line(unsigned long *lpPixelBufferLine);
			
BITS 32

SECTION .text			
global _Averaging_MMX_line;

_Averaging_MMX_line:
	push ebp
	mov ebp, esp
	sub esp, 0x10;  
	push edi
	push esi
	push eax
	push ebx
	push ecx
	push edx

	%define lpPixelBufferLine [dword ebp+0x08]

	pop edx
	pop ecx
	pop ebx
	pop eax
	pop esi
	pop edi

	emms
	leave 
	ret 