.model small
.386
.stack 256
.data
    buf db 600 dup(0)

.code
main:
    mov ax, @data
    mov ds, ax

    mov ah, 19h
    int 21h

    mov cx, 01h
    mov dx, 00h
    lea bx, buf
    int 25h
    jc error
error:
    mov ax, 4c00h
    int 21h
end main
