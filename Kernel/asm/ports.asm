GLOBAL _port_write_byte
GLOBAL _port_write_word
GLOBAL _port_read_byte
GLOBAL _port_read_word
GLOBAL inpw
GLOBAL outpw
GLOBAL _song_note

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

; TODO: Doc
align 16
_port_write_byte:
	pushaq					; Backup everything

	xor rax, rax			; Clean registers
	xor rdx, rdx

	mov rax, rsi
	and rax, 0FFh 			; Last byte
	mov rdx, rdi
	and rdx, 0FFFFh 		; Last word
	out dx, al

	popaq					; Restore everything
	ret

align 16
_port_write_word:
	pushaq					; Backup everything

	xor rax, rax			; Clean registers
	xor rdx, rdx

	mov rax, rsi
	and rax, 0FFFFh 		; Last word
	mov rdx, rdi
	and rdx, 0FFFFh 		; Last word
	out dx, ax

	popaq					; Restore everything
	ret

align 16
_port_read_byte: ; TODO: Test & Doc
	pushfq					; Backup everything
	push rdx

	xor rax, rax			; Clean registers
	xor rdx, rdx

	mov rdx, rdi
	and rdx, 0FFFFh 		; Last word
	in al, dx

	pop rdx					; Restore everything
	popfq
	ret

align 16
_port_read_word: ; TODO: Test & Doc
	pushfq					; Backup everything
	push rdx

	xor rax, rax			; Clean registers
	xor rdx, rdx

	mov rdx, rdi
	and rdx, 0FFFFh 		; Last word
	in ax, dx

	pop rdx					; Restore everything
	popfq
	ret

; out for video function
outpw:
	mov rdx, rdi
	out dx, ax
	ret
; in for video function
inpw:
	mov rdx, rdi
	in ax, dx
	ret
	

_song_note:
	mov     al, 182         ; Prepare the speaker for the
	out     43h, al         ;  note.
	xor		rax, rax
	mov     rax, rdi        ; Frequency number (in decimal)
	out     42h, al         ; Output low byte.
	mov     al, ah          ; Output high byte.
	out     42h, al 
	in      al, 61h         ; Turn on note (get value from
		                                ;  port 61h).
	or      al, 00000011b   ; Set bits 1 and 0.
	out     61h, al         ; Send new value.
	mov     bx, si          ; Pause for duration of note.
	
	.pause1:
		mov     cx, 65535
		
	.pause2:
	dec     cx
	jne     .pause2
	dec     bx
	jne     .pause1
	in      al, 61h         ; Turn off note (get value from
							;  port 61h).
	and     al, 11111100b   ; Reset bits 1 and 0.
	out     61h, al         ; Send new value.
	ret 