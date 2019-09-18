global start

%define __exit 0x2000001

start:
    mov rax, 0x4
    mov rbx, 0x3
    add rax, 0x7

    mov rax, __exit
    xor rdi, rdi
    syscall
