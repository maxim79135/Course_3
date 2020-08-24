.model small

.data
	msg db 'Symbol $', 0Dh, 0Ah, '$'
	buffer db 256 dup('$')
	str db 1 dup('-')
	len equ ($ - buffer)
.stack 256h

.code


gotoxy macro xord, yord
	mov ah, 2h
	mov bh, 0
	mov dh, yord
	mov dl, xord
	int 10h
endm

print macro string
	mov ah, 9h
	lea dx, string+1
	int 21h
endm

print_l macro string
	mov ah, 9h
	lea dx, string+2
	int 21h
endm


main:
	mov ax, @data
	mov ds, ax
	
	mov ax, 3h ;clean window
	int 10h
	
	mov ah, 9h
	lea dx, msg
	int 21h

	mov ah, 0Ah
	lea dx, buffer
	int 21h

	;mov buffer+1 0Ah

	mov ax, 3h
	int 10h	


	xor al, al
	lea si, buffer+2
	mov cx, 0
	counting:
		cmp byte ptr [si], '$'
			je cont
			inc cx
			inc si
			jmp counting
	cont:

	mov ah, 9h
	mov bl, 5h 
	lea dx, buffer+2
	int 10h
	int 21h


	mov cx, 0

	point:
		mov al, 0h
		int 10h

		gotoxy cx, 0
		mov ah, 10h
		int 16h

		cmp ah, 04Dh
		je right
		cmp ah, 04Bh
		je left 
		cmp ah, 01h
		je exit	
		
	right:	
		print buffer
		inc cx
		jmp point

	left:
		cmp cx, 0
		je point

		mov al, 0h
		int 10h		
		print_l buffer
		dec cx	
		jmp point


	exit:
		mov ah, 4ch
		int 21h

end main
