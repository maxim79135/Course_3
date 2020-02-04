.386
.model flat
    extrn MessageBoxA: dword
    extrn ExitProcess: dword
    extrn GetUserNameA: dword
.code
_start:
    push offset nSize
    push offset lpBuffer
    call GetUserNameA
    push 40h
    push offset Msg
    push offset lpBuffer
    push 0
    call MessageBoxA
    push 0
    call ExitProcess
.data
    Msg db 'laba6',0
    lpBuffer db 256 dup(0)
    nSize dw 256
end _start