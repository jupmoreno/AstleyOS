GLOBAL _cmos_write
GLOBAL _cmos_read

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

;--------------------------------------------------------------
; Writes the given value at the desired CMOS address          -
; @param rdi - the address to be written  		              -
; @param rsi - the value to be written 		                  -
;--------------------------------------------------------------
align 16
_cmos_write:
	pushaq

	mov rax, rdi

	cli

	out 0x70, al ; sets the address to write
	call _delay
	mov rax, rsi
	out 0x71, al ; writes the value at the set address

	sti

	popaq
	ret
;--------------------------------------------------------------

;--------------------------------------------------------------
; Reads from the CMOS address		                      	  -
; @param rdi - the address to be read  		                  -
; @return rax - the read value								  -
;--------------------------------------------------------------
align 16
_cmos_read:
	push rbp
	mov rbp, rsp

	mov rax, rdi

	cli

	out 0x70, al 		; set the address to read
	call _delay
	xor rax, rax
	in al, 0x71 		; reads the set address 

	sti 

	mov rsp, rbp
	pop rbp
	ret
;--------------------------------------------------------------

;--------------------------------------------------------------
; Delays WAITING_TIME units the process that called 		  -
; this function.	       									  -
;--------------------------------------------------------------
_delay:
	pushaq
	mov rcx, WAITING_TIME

.loop:
	cmp rcx, 0
	jz .end
	dec rcx
	jmp .loop

.end:
	popaq
	ret
;--------------------------------------------------------------

section .data
	WAITING_TIME db 1