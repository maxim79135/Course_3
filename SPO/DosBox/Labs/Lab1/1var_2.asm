.model      small
.data

message2    db  0ah,0dh,'String:$'
message3    db  0ah,0dh,'Result:$'

FullData    STRUC
    one db 20 dup(24h)
FullData    ENDS
FD FullData 1 dup(<>)

.stack      256h
.code

MESSAGE proc near
    push    ax
    push    cx
    mov ah,09h
    int 21h
    pop cx
    pop ax
    ret
MESSAGE endp

INPUTSTR proc near
    xor di,di   
m1:
    mov ah,0
    int 16h
    cmp al,0
    jne m3
    mov ah,0
    int 16h
    jmp m1
m3:
    cmp dx,1
    je  m4
    cmp al,42
    je  quit
m4:
    cmp al,33
    jle m1
    mov [bx+di],al
    inc di
    mov ah,2
    mov dl,al   
    int 21h
    loop    m1
quit:
    mov al, 24h
    mov [bx+di], al
    ret
INPUTSTR endp

FINDLENGTH proc near
    mov al, 24h
    xor cx, cx
    lea di,FD.one
findlength_m1:
    cmp [di], al
    je findlength_quit
    inc cx
    inc di
    jmp findlength_m1
findlength_quit:
    ret
FINDLENGTH endp

SORTUP proc near
    xor si, si
    xor di, di
    lea bx, FD.one
    mov ax, cx
sortup_i:
    xor di, di
    mov cx, ax
    sub cx, si
    dec cx
    jz sortup_quit
sortup_j:
    mov dl, [bx+di]
    cmp [bx+di+1], dl
    jle sortup_next_loop
    jmp sortup_nothing
sortup_next_loop:
    mov dh, [bx+di+1]
    mov [bx+di+1], dl
    mov [bx+di], dh
sortup_nothing:    
    inc di
    cmp di, cx
    jne sortup_j
    inc si
    cmp si, ax
    jne sortup_i
sortup_quit:
    ret
SORTUP endp


main:
    mov ax,@data
    mov ds,ax


    mov ah, 0
    mov bh, 0
    mov ch, 0
    mov dh, 0

    mov ax, 0
    mov bx, 0
    mov cx, 0
    mov dx, 0

    lea dx,message2
    call    MESSAGE
    lea bx,FD.one
    xor dx,dx
    call    INPUTSTR

    call FINDLENGTH
    call    SORTUP
    lea dx,message3
    call    MESSAGE
    lea dx, FD.one
    call MESSAGE
        

    mov ah,4ch
    int 21h
end main

