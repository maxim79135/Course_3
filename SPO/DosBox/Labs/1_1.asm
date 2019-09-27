.model small
.data
    message db 'Hello: $'

.stack 256h
.code

main:
    mov ax, @data
    mov ds, ax

    mov ah, 00h
    mov al, 10h
    int 10h

    lea dx, message
    mov ah, 09h
    int 21h
    
    ; exit program
    mov ah, 4ch
    int 21h
end main