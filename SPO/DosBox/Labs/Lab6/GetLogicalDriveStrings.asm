.386
.model flat
extrn GetLogicalDriveStringsA: dword
extrn MessageBoxA: dword
extrn ExitProcess: dword
 
.code
_start:
 push offset uSize
 push offset lpBuffer
 call GetLogicalDriveStringsA
 
 push 40h
 push offset App
 push offset lpBuffer
 push 0
 call MessageBoxA
 
 push 0
 call ExitProcess
.data
 App db 'PE Linker Test', 0
 lpBuffer db 260 dup(0)
 uSize dw 0

end _start