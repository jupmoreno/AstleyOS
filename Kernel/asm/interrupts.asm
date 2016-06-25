GLOBAL _interrupt_set
GLOBAL _interrupt_clear

GLOBAL _interrupt_20
EXTERN interrupt_20

GLOBAL _interrupt_21
EXTERN interrupt_21

GLOBAL _interrupt_80
EXTERN interrupt_80

GLOBAL _startProcess

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
; Enables INTR hardware interrupts.
; -----------------------------------------------------------------------------
align 16
_interrupt_set:
	sti
	ret
; -----------------------------------------------------------------------------

; -----------------------------------------------------------------------------
; Disables INTR hardware interrupts.
; -----------------------------------------------------------------------------
align 16
_interrupt_clear:
	cli
	ret
; -----------------------------------------------------------------------------

; -----------------------------------------------------------------------------
; // TODO: 
; -----------------------------------------------------------------------------
align 16
_interrupt_20:
	pushaq
	mov rdi, rsp
	call interrupt_20
	cmp rax, 0
	jnz curry
	mov rsp, rax
	popaq
	
.curry: iretq
; -----------------------------------------------------------------------------

; -----------------------------------------------------------------------------
; // TODO: 
; -----------------------------------------------------------------------------
align 16
_interrupt_21:
	pushaq

	xor rax, rax		; To ensure rax register will be empty

	in al, 0x60			; Get the scancode from the keyboard
	mov rdi, rax
	call interrupt_21

	popaq
	iretq
; -----------------------------------------------------------------------------

; -----------------------------------------------------------------------------
; // TODO: 
; -----------------------------------------------------------------------------
align 16
_interrupt_80:
	push rbp
	mov rbp, rsp

	sti							; // TODO: Why?? 

    push rdi
    mov rdi, rax
    call interrupt_80
    pop rdi

    cmp rax, 0
    je interrupt_80_error
    call rax

interrupt_80_ret:
    mov rsp, rbp
	pop rbp

    iretq

interrupt_80_error:
	mov rax, -1					; // TODO: Error code?
	jmp interrupt_80_ret
; -----------------------------------------------------------------------------
