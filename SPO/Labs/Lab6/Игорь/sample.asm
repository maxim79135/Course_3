	.386
	
	.model flat
	extrn	GetDoubleClickTime:DWORD
	
	extrn	MessageBoxA:DWORD
	extrn	ExitProcess:DWORD
	
	.code
_start:
	call	GetDoubleClickTime
	push	40h
	push	offset App
	lea    EBX,dop
    	mov    dword ptr [EBX], dword ptr EAX
	xor dx, dx
 	lea edi, dop
 	add edi, size dop
 	dec edi
 	mov byte ptr [edi], 0
	mov bx, 10
	@z: div bx
 	add dl, 30h
 	dec edi
 	mov byte ptr [edi], dl
 	xor dx, dx
 	test ax, ax
 	jnz @z
    	push    offset dop
	push	0	
	call	MessageBoxA
	push	0
	call	ExitProcess

	.data	
	
App	db	'PE Linker Test', 0
dop    db     49 dup(33),0
		
	end	_start