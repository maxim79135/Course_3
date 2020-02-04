.386
.model flat
extrn MoveFile: dword
extrn MessageBoxA: dword
extrn ExitProcess: dword
 
.code
_start:
 push offset secondname
 push offset firstname
 call MoveFile

 push 40h
 push offset App
 cmp eax, 0
 je m1
 push offset succes
 jmp m2
m1:
 push offset error
m2:
 push 0
 call MessageBoxA
 
 push 0
 call ExitProcess
.data
 App db 'PE Linker Test', 0
 firstname db 10, 0
 secondname db 10, 0
 succes db 'Файл изменен', 0
 error db 'Ошибка при изменении',0

end _start
