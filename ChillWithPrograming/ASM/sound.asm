; masm dos com #
.model tiny
.286 
.data
;tek_mel - адрес текущей ноты выбранной мелодии
;Мелодия "Чижик Пыжик"
mel1 db 17,17,255,13,13,255,17,17,255,13,13,255,18,18,255
db 17,17,255,15,15,15,15,255,255
db 8,8,255,8,8,255,8,8,255,10,255,12,255
db 13,13,255,13,13,255,13,13,13,13
db 0 
;Мелодия "Подмосковные вечера"
mel2 db 1,1,1,4,4,4,8,8,8,4,4,4,6 dup(6),4,4,4,3,3,3
db 6 dup(8),6 dup(6),12 dup(1)
db 0 
menu db 10 dup(0ah), 30 dup(20h), 'Enter the point of menu:'
db 0Dh,0Ah,30 dup(20h),'1 - CHIZHIK-PYZHIK',0Dh,0Ah,30 dup(20h),'2 - PODMOCKOVNYE VECHERA',0dh,0ah,30 dup(20h), 'q - Quit',5 dup(0Ah),"$"
er db 'Incorrect input! Try again!',0Dh,0Ah,"$"
;Коэффициенты деления для нот
noty dw 0eeeh,0e18h,0d49h,0c8eh,0bdfh,0b2fh,0abeh
dw 9f7h,968h,8e0h,861h,7e8h,777h,70ch,6a5h,647h
dw 5edh,597h,547h,4fbh,4b4h,470h,430h,3f4h
dw 3bbh,386h,352h,323h,2f6h,2cbh,2a3h,27dh,25ah,238h,218h,1fah
dw 1ddh,1c3h,1a9h,192h,17bh,166h,152h,13fh,12dh,11ch,10ch,0fdh
dw 0 
iniflag db 0; Флаг звучания 
old_int1c_off dw 0; Смещение старого вектора
old_int1c_seg dw 0; Сегмент старого вектора
tek_mel dw ?; Адрес текущей ноты выбранной мелодии 

.code
org 100h
Start:  
    mov ax,351Ch 
    int 21h
    mov old_int1c_off,bx 
    mov old_int1c_seg,es
    lea dx,new_int1c 
    mov ax,251Ch 
    int 21h; DS:DX – адрес новой программы обр. 
    mov ax, @data
    mov ds, ax
    mov ah,9 
    lea dx,menu 
    int 21h
beg1:   mov ah,0 ;Опрос клавиатуры и проверка на выбор мелодии
    int 16h
    cmp al,'1' 
    jnz beg2 ; Нет
    or iniflag,1; Взведение флага звуч.
    lea ax,mel1
    mov tek_mel,ax
    jmp beg1; Переход на начало цикла 
beg2:   cmp al,'2' 
    jnz beg3 
    or iniflag,1 
    lea ax,mel2
    mov tek_mel,ax
    jmp beg1
beg3:   cmp al,'q' 
    jz beg5
beg4:   mov ah,9
    lea dx,er
    int 21h 
    jmp beg1 
beg5:;Восстановление старого вектора 1Ch и выход
    mov dx,old_int1c_off 
    mov ds,old_int1c_seg ;DS:DX – адрес устанавл. вектора
    mov ax,251Ch 
    int 21h 
    mov ah,4Ch
    int 21h


new_int1c proc far;Новый обработчик прерывания 1ch
    pusha   ; Вызов процедура извлечения звука
    test cs:iniflag,1; Проверка флага
    jnz muz1 
muze:   in al,61h ; Чтение состояния системного порта В
    and al,0FCh; Запрещение звучания (биты 0 и 1)
    out 61h,al; Запись в системный порт В
    jmp ex ; Выход, если флаг не взведен
muz1:   mov si,cs:tek_mel; Адрес текущей ноты
    mov bl,cs:[si]; Текущая нота
    cmp bl,255; Пауза?
    jnz muz2 
;Выключение звука
    in al,61h 
    and al,0FCh 
    out 61h,al 
    inc cs:tek_mel; Переход к адресу след. ноты
    jmp ex
muz2:   or bl,bl; = 0?
    jz muze 
muz3:   xor bh,bh 
    mov ax,cs:noty [bx]
;Программирование делителя частоты 2 канала
    inc cs:tek_mel; Переход к адресу след. ноты
    out 42h,al; Мл.байт частоты. канал 2 таймера
    xchg al,ah; AH. AL
    out 42h,al; Ст. байт частоты. канал 2 таймера 
;Разрешение звучания
    in al,61h 
    or al,3; Разрешение звучания (биты 0 и 1)
    out 61h,al; Запись в системный порт В
ex:     popa
    iret
new_int1c endp;Процедура извлечения очередного звука
END Start