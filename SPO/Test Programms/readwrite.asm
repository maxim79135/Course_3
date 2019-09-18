BITS 64

%define __read 0x2000003
%define __write 0x2000004
%define __exit 0x2000001

GLOBAL start

SECTION .data
    userMsg db 'Please enter a number: '
    lenUserMsg equ $ - userMsg
    userTxt db ''

SECTION .bss
    %define bufsize 1024
    buffer resb bufsize
    num resb 5

SECTION .text

start:
    mov rax, __write
    mov rdi, 1
    mov rsi, userMsg
    mov rdx, lenUserMsg
    syscall

    ;read value1
    mov rax, __read
    xor rdi, rdi
    mov rsi, buffer
    mov rdx, bufsize
    mov rcx, num
    mov rbx, 0x2
    syscall
    push rax ;write first value

    ;read value_2
    mov rax, __read
    xor rdi, rdi
    mov rsi, buffer
    mov rdx, bufsize
    syscall
    push rax ;write first value

    pop rbx
    pop rax
    add rax, rbx

    mov rcx, rax
    mov rdx, 0x5
    mov rax, __write
    mov rdi, 1
    syscall

    mov rax, __exit
    xor rdi, rdi
    syscall
