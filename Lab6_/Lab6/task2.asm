.386
.model flat,stdcall
    extrn MessageBoxA: dword
    extrn ExitProcess: dword
    extrn GetComputerNameA: dword
.code
_start:
    push offset nSize
    push offset lpBuffer
    call GetComputerNameA
    push 40h
    push offset Msg
    push offset lpBuffer
    push 0
    call MessageBoxA
    push 0
    call ExitProcess
.data
    lpBuffer db 256 dup(0),0
    Msg db 'laba6',0
    nSize dw 256
end _start
