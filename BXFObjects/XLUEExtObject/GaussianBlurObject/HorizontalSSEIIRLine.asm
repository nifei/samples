; Stretch SSE main loop implement

;float horizontal_sse_iir_line(float *oTemp,  unsigned long* id, float *od, int width, int height, float *a0, float *a1, float *b1, float *b2, float *cprev);
			

BITS 32

SECTION .text			
global _horizontal_sse_iir_line

_horizontal_sse_iir_line:
	push ebp
	mov ebp, esp
	
	%define otemp [dword ebp + 8]
	%define id [dword ebp + 0x0c]
	%define od [dword ebp + 0x10]
	%define width [dword ebp + 0x14]
	%define a0 [dword ebp+0x1c]
	%define a1 [dword ebp+0x20]
	%define b1 [dword ebp+0x24]
	%define b2 [dword ebp+0x28]
	%define cprev [dword ebp+0x2c]
	
	sub esp, 0x18
	push edi
	push esi
	push eax
	push ebx
	push ecx
	push edx

	mov eax, otemp;
	mov eax, id;
	mov eax, od;
	mov eax, width;
	mov eax, a0;
	mov eax, a1;
	mov eax, b1;
	mov eax, b2;
	mov eax, cprev;

	pop edx
	pop ecx
	pop ebx
	pop eax
	pop esi
	pop edi

	leave 
	ret 