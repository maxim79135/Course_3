    .386        ;????????? ???-31    
    
    .model flat, stdcall
    
    extrn   MessageBoxA:DWORD
    extrn   ExitProcess:DWORD
    extrn   IsCharLowerA:DWORD
    
    .code
_start:ff
    lea     edi, Char
    push    [edi]
    call    IsCharLowerA
    cmp     eax,0
    je      label1
    jmp     closes
label1:
    lea     edi, Charb
closes:
    push    40h
    push    offset App
    push    edi
    push    0   
    call    MessageBoxA
    push    0
    call    ExitProcess

    .data   
Char    db  'a'
Chara db 'True', 0
Charb db 'False',0 
App db  'PE Linker Test', 0
        
    end _start