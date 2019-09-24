.model small
.data
    message1 db 0ah, 0dh, 'Symbol: $'
    message2 db 'String1: $'
    message3 db 0ah, 0dh, 'String2: $'
    message4 db 0ah, 0dh, 'String3: $'
    message5 db 0ah, 0dh, 'Result1: $'
    message6 db 0ah, 0dh, 'Result2: $'
    message7 db 0ah, 0dh, 'Result3: $'
    
    FullData STRUC
        one db 11 dup(24h)
        two db 11 dup(24h)
        three db 11 dup(24h)
        four db 11 dup(24h)
        character db 1 dup(24h)
    FullData ENDS
    FD FullData 1 dup(<>)

.stack 256h
.code

GETCHAR proc near
    push ax
    push dx
getchar_m1:
    mov ah, 08h
    int 21h
    cmp al, 00h
    jne getchar_m3
    mov ah, 08h
    int 21h
    jmp getchar_m1
getchar_m3:
    cmp dx, 01h
    jne getchar_m4
    cmp al, 0dh
    je getchar_quit
getchar_m4:
    cmp al, 33
    jle getchar_m1
getchar_quit:
    mov [bx], al
    mov ah, 02h
    mov dl, al
    int 21h
    pop dx
    pop ax
    ret
GETCHAR endp 

GETSTRING proc near
    mov cx, 0ah
    mov dx, 01h

getstring_m1:
    call GETCHAR
    cmp byte ptr [bx], 0dh
    je getstring_m2
    inc bx
    loop getstring_m1

getstring_m2:
    ret
GETSTRING endp

MESSAGE proc near
    push ax
    push cx
    mov ah, 09h
    int 21h
    pop cx
    pop ax
    ret
MESSAGE endp

main:
    mov ax, @data
    mov ds, ax
    
    ; print string1
    lea dx, message2
    call MESSAGE
    
    ; read string1
    lea bx, FD.one
    call GETSTRING
    
    ; print string2
    lea dx, message3
    call MESSAGE
    
    ; read string2
    lea bx, FD.two
    call GETSTRING

    ; print string3
    lea dx, message4
    call MESSAGE
    
    ; read string3
    lea bx, FD.three
    call GETSTRING

    ; print Symbol
    lea dx, message1
    call MESSAGE
    
    ; read character
    lea bx, FD.character
    xor dx, dx
    call GETCHAR

    ; find character in each string
    mov al, FD.character
    xor dx, dx
    mov cx, 0ah
    lea bx, FD.one

main_m1:
    cmp [bx], al
    jne main_m2
    inc dx
main_m2:
    inc bx
    loop main_m1

    lea di, FD.one
    cmp dx, 00h
    jne main_print_string

    lea di, FD.four
    mov [di], al
    inc di

    ; copy other characters
    mov cx, 0Ah
    lea si, FD.one
main_m3:
    mov ah, [si]
    mov [di], ah
    inc di
    inc si
    loop main_m3
    mov byte ptr [di], 24h
    lea di, FD.four

main_print_string:
    lea dx, message5
    call MESSAGE
    mov dx, di
    call MESSAGE
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; exit program
    mov ah, 4ch
    int 21h
end main