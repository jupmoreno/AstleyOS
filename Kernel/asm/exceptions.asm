GLOBAL _exception_0E
EXTERN exception_0E

section .text

align 16
%macro pushaq 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
	pushfq
%endmacro

align 16
%macro popaq 0
	popfq
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

; -----------------------------------------------------------------------------
; // TODO:
; -----------------------------------------------------------------------------

_exception_0E:	; Stack Fault
	pushaq

	mov eax, [rsp+136] ; error code
	mov rdi, rax
	mov rsi, cr2
	call exception_0E

	popaq
	iretq

; -----------------------------------------------------------------------------
