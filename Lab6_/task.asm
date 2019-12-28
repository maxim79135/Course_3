.386
.model flat
    extrn MessageBoxA: dword
    extrn ExitProcess: dword
    extrn GetComputerNameA: dword
.code
_start:
    push offset nSize
    push offset lpBuffer
    call GetComputerNameA
    push 40h
    push offset lpBuffer
    push offset Msg
    push 0
    call MessageBoxA
    push 0
    call ExitProcess
.data
    lpBuffer db 100 dup(0),0
    nSize db 20 dup(0),0
    Msg db 'PE Linker',0
end _start
