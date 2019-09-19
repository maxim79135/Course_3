.model small
.data
    message1 db 'Symbol:$'
    message2 db 0ah,0dh,'Stroka:$'
    message3 db 0ah,0dh,'Result:$'
    
    FullData STRUC
        one db 9 dup(36)
        two db 10 dup(36)
        three db 1 dup(36),36
    FullData ENDS
    FD FullData 1 dup(<>)

.stack 256h
.code
GETCHAR proc near
    push ax
    push dx
    xor di, di
m1:
    mov ah, 8h
    int 21h
    cmp al, 0
    jne m3
    mov ah, 8h
    int 21h
    jmp m1
m3:
    cmp dx, 1
    je m4
    cmp al, 13
    je quit
m4:
    cmp al, 33
    jle m1
    mov [bx], al
    mov ah, 2
    mov dl, al
    int 21h
    pop dx
    pop ax
quit:
    ret
GETCHAR endp 

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
    lea dx, message2
    
    call MESSAGE
    
    lea bx, FD.three
    xor dx, dx
    call GETCHAR
    
    mov ax, 4c00h
    int 21h
end main