<<<<<<< HEAD
.386
.model flat
	extrn 	GetUserNameA:DWORD
	extrn	MessageBoxA:DWORD
	extrn	ExitProcess:DWORD

.code

_start:
    push    50
    push    offset buf
    call    GetUserNameA

	push	40h	
	push	offset App
	push    offset buf
	push	0	

	call	MessageBoxA

	push	0
	call	ExitProcess

.data	
	App	db	'PE Linker Test', 0
	buf db  50 dup(?)
=======
.386
.model flat
	extrn 	GetUserNameA:DWORD
	extrn	MessageBoxA:DWORD
	extrn	ExitProcess:DWORD

.code

_start:
    push    50
    push    offset buf
    call    GetUserNameA

	push	40h	
	push	offset App
	push    offset buf
	push	0	

	call	MessageBoxA

	push	0
	call	ExitProcess

.data	
	App	db	'PE Linker Test', 0
	buf db  50 dup(?)
>>>>>>> 91850d5489a9fde5dd70a43ca26aefc90603e79e
end	_start