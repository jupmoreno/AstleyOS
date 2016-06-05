GLOBAL _cr0_read:
GLOBAL _cr0_write:
GLOBAL _cr3_read:
GLOBAL _cr3_write:

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

align 16
_cr0_read:
	mov rax, cr0

	ret

align 16
_cr0_write:
	mov cr0, rdi

	ret

align 16
_cr3_read:
	mov rax, cr3

	ret

align 16
_cr3_write:
	mov cr3, rdi

	ret
