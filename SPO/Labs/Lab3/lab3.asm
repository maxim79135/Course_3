;Караваев Павел, ИВТ-32
;Вариант 16
;Вывести список файлов в текущем каталоге, используя атрибуты цвета, создать файл и снова вывести список
.model small
.data

Packet STRUC					;Структура для использования прерываний 25/26
	sec			db 4 dup(0)		;сектор для чтения
	number		db 2 dup(0)		;число секторов для чтения
	transfer	db 4 dup(0)		;Адрес буфера в виде сегмент:смещение
Packet ENDS

bootsec STRUC					;Структура из необходимых для работы данных, получаемых из загрузочной записи
	sectorsize	db 2  dup(0)    ;размер Сектора в байтах
	sectors		db 1  dup(0)    ;Число секторов в кластере
	reserved	db 2  dup(0)    ;Число зарезервированных секторов
	fatnum		db 1  dup(0)    ;Число FAT таблиц
	rootentr	db 2  dup(0)    ;Число записей корневого каталога
	fatsecs		db 2  dup(0)    ;Число секторов на таблицу
bootsec ENDS

;имена каталогов и файла
folder 	db	'        '	

pack Packet 1 dup(<>)

fol	dd 4		;Кластер папки

folold dd 4		;Не используется хуйня


boot 	bootsec 1 dup(<>)

root 	dd 4

tmpcx   dw 2

tmpsec dd 4

buf db 4096 dup(?)
		db '$'
		
pathbuf db 64 dup('\'), 0

fatbuf db 512 dup(?)
		db '$' 
		

;ВСТАВИТЬ ВВОД ТЕКУЩЕГО КАТАЛОГА, ВСТАВИТЬ ПРОВЕРКУ НА ФОРМАТ СОМ И ВЫВОДИТЬ ЕГО ДВУМЯ ЦВЕТАМИ


.stack 256h

.code

jmp Main

findfat proc near
	lea si, fol
	mov byte ptr [si], 0
	xor si,si
SearchFAT:
	inc si
	lea bx, pack								;Загрузка структуры
	mov word ptr [bx], si						;МЛАДШИЕ РАЗРЯДЫ НОМЕРА СЕКТОРА
	mov word ptr [bx+2], 0						;СТАРШИЕ РАЗРЯДЫ НОМЕРА СЕКТОРА
	mov word ptr [bx+4], ax						;ЧИСЛО СЕКТОРОВ
	mov word ptr [bx+6], offset fatbuf		;СМЕЩЕНИЕ НА БУФЕР
	mov word ptr [bx+8], seg fatbuf			;СЕГМЕНТ БУФЕРА
	mov cx, 0FFFFh								;Указание, что объем накопителя больше 32 мегабайт
	mov al, 03h									;выбор активного диска(D)
	int 25h										;Чтение сектора
	pop ax;

	xor di, di
SearchFATSector:
	cmp di, 512
	je SearchFAT
	mov ax, word ptr [fatbuf+di];поиск свобоного места
	cmp ax, 0000h
	je FoundEmptyFATSector
	inc di
	inc di
	push si
	lea si, fol
	add byte ptr [si], 1
	pop si
	jmp SearchFATSector

FoundEmptyFATSector:
	mov word ptr [fatbuf+di], 0FFFFh
	inc di
	inc di
	mov word ptr [fatbuf+di], 0FFFFh
	dec di
	dec di
	lea bx, pack								;Загрузка структуры
	mov word ptr [bx], si						;МЛАДШИЕ РАЗРЯДЫ НОМЕРА СЕКТОРА
	mov word ptr [bx+2], 0						;СТАРШИЕ РАЗРЯДЫ НОМЕРА СЕКТОРА
	mov word ptr [bx+4], 1						;ЧИСЛО СЕКТОРОВ
	mov word ptr [bx+6], offset fatbuf		;СМЕЩЕНИЕ НА БУФЕР
	mov word ptr [bx+8], seg fatbuf			;СЕГМЕНТ БУФЕРА
	mov cx, 0FFFFh								;Указание, что объем накопителя больше 32 мегабайт
	mov al, 03h									;выбор активного диска(D)
	int 26h										;Чтение сектора
	pop ax;
	
	push di
	push si
	lea si,	boot
	mov di, [si+8]						;ЧИТАЕМ В AX РАЗМЕР ФАТ ТАБЛИЦЫ В СЕКТОРАХ	
	pop si
	mov ax, si
	add ax, di
	pop di
	
	lea bx, pack								;Загрузка структуры
	mov word ptr [bx], ax						;МЛАДШИЕ РАЗРЯДЫ НОМЕРА СЕКТОРА
	mov word ptr [bx+2], 0						;СТАРШИЕ РАЗРЯДЫ НОМЕРА СЕКТОРА
	mov word ptr [bx+4], 1						;ЧИСЛО СЕКТОРОВ
	mov word ptr [bx+6], offset fatbuf		;СМЕЩЕНИЕ НА БУФЕР
	mov word ptr [bx+8], seg fatbuf			;СЕГМЕНТ БУФЕРА
	mov cx, 0FFFFh								;Указание, что объем накопителя больше 32 мегабайт
	mov al, 03h									;выбор активного диска(D)
	int 26h										;Чтение сектора
	pop ax;
	ret
findfat endp

fatnext proc near
	push si
	push di
	push cx
	
	lea si, fol
	mov byte ptr [si], 0
	xor si,si
	xor cx, cx
SearchNextFAT:
	push cx
	inc si
	lea bx, pack								;Загрузка структуры
	mov word ptr [bx], si						;МЛАДШИЕ РАЗРЯДЫ НОМЕРА СЕКТОРА
	mov word ptr [bx+2], 0						;СТАРШИЕ РАЗРЯДЫ НОМЕРА СЕКТОРА
	mov word ptr [bx+4], ax						;ЧИСЛО СЕКТОРОВ
	mov word ptr [bx+6], offset fatbuf		;СМЕЩЕНИЕ НА БУФЕР
	mov word ptr [bx+8], seg fatbuf			;СЕГМЕНТ БУФЕРА
	mov cx, 0FFFFh								;Указание, что объем накопителя больше 32 мегабайт
	mov al, 03h									;выбор активного диска(D)
	int 25h										;Чтение сектора
	pop ax;
	pop cx
	xor di, di
SearchNextFATSector:
	cmp di, 512
	je SearchNextFAT
	cmp cx, word ptr fol
	je FoundSector
	inc di
	inc di
	inc cx
	jmp SearchNextFATSector

FoundSector:
	
					;В начале происходит вычисление. Из номера кластера вычитается двойка, умножается на число секторов в кластере. 
					;К результату необходимо добавить смещение системной области
	xor ax, ax
	xor cx, cx
	lea si, fol
	mov ax, [si]
	sub ax, 2
	lea si, boot		
	mov cl, [si+2]	;sectors per cluster
	mul cx
	push ax		;пусть ax пока полежит подождет
	
	mov ax, [si+6]	;root entries
	mov cx, 32
	mul cx
	mov cx, [si]	;bytes per sectors
	div cx
	
	pop dx
	add ax, dx		;добавим, что сохраняли. Это был отступ
	lea si, root
	mov bx, [si]
	add ax, bx
	
	;начало каталога вычислено
	;Загрузка каталога
	lea si, tmpsec
	mov [si], ax
	
	lea si, pack
	mov word ptr [si], ax
	mov word ptr [si+2], 0
	mov word ptr [si+4], 8	
	mov word ptr [si+6], offset buf
	mov word ptr [si+8], seg buf
	mov bx, si
	mov CX, 0FFFFh
	mov ax, 0003h
	int 25h
	pop ax
	
	;чтение каталога
	lea di, buf

	pop cx
	pop di
	pop si
	
	ret
fatnext endp

output proc near
	xor ax, ax
	lea di, buf
	add di, 64 ;первые 2 "файла" описывают саму папку и родительскую, можно скипнуть
	mov ah, 00h
	mov al, 03h
	int 10h         ; set text video mode, 80x25
newtmp:
	mov tmpcx, 0
	MOV     AX,3H   ; Not really needed, but it
    INT     10H     ; clears the screen.
next:
	mov cx, tmpcx
	cmp cx, 25
	je newtmp
	mov ax, [di]
	cmp ax, 00h
	je skip;
	cmp word ptr [di], 0e5h
	je skip;
    MOV     AL,1    ; Move cursor,
	
    MOV     BX,04   ; BL Attribute, BH Page Number,
    MOV     CX, 11; CX Sting Length, 
	lea si, tmpcx
    MOV     DH, [si]    ; DH Row to start displaying (zero based),
    MOV     AH,13H  ; Video BIOS Print String function,
    MOV     DL,0    ; DL Column to start displaying,
	mov bp,di
    INT     10H     ; Video BIOS interrupt
	MOV     AH,10H  ; Read Extended Keyboard Input.
    INT     16H     ; {Used for a pause.  Not needed, but makes the
                        ; message legitimate.}
	inc tmpcx
skip:
	lea ax, pathbuf   ;костыль для проверки на переход в следующий кластер
	dec ax			;pathbuf лежит сразу после buf, поэтому так делать можно
	add di, 32
	
	cmp di, ax		;
	jl next;	
	ret
output endp

init proc near 									;процедура чтения загрузочной записи и загрузка корневого каталога в память
	mov al, 04h									;выбор активного диска(D) (04h E)
	lea bx, pack								;Загрузка структуры
	mov word ptr [bx], 0						;МЛАДШИЕ РАЗРЯДЫ НОМЕРА СЕКТОРА
	mov word ptr [bx+2], 0						;СТАРШИЕ РАЗРЯДЫ НОМЕРА СЕКТОРА
	mov word ptr [bx+4], 8						;ЧИСЛО СЕКТОРОВ
	mov word ptr [bx+6], offset buf				;СМЕЩЕНИЕ НА БУФЕР
	mov word ptr [bx+8], seg buf				;СЕГМЕНТ БУФЕРА
	mov cx, 0FFFFh								;Указание, что объем накопителя больше 32 мегабайт
	int 25h										;Чтение сектора
	pop ax;
	;ТАК, А ТЕПЕРЬ ДАВАЙ ПОЧИТАЕМ ОТСЕДОВА
	
	lea si, buf									;Переход на считанный сектор
	lea bx, boot								;Получение адреса структуры загрузочной записи
	mov dx, [si+0Bh]
	mov word ptr [bx]  , dx				 ;РАЗМЕР СЕКТОРА В БАЙТАХ
	
	mov dx, [si+0Dh]
	mov byte ptr [bx+2], dl              ;РАЗМЕР КЛАСТЕРА В СЕКТОРАХ
	
	mov dx, [si+0Eh]
	mov word ptr [bx+3], dx              ;ЧИСЛО ЗАРЕЗЕРВИРОВАННЫХ СЕКТОРОВ
	
	mov dx, [si+10h]
	mov byte ptr [bx+5], dl              ;ЧИСЛО FAT таблиц
	
	mov dx, [si+11h]
	mov word ptr [bx+6], dx              ;ЧИСЛО ЗАПИСЕЙ В РУТЕ
	
	mov dx, [si+16h]
	mov word ptr [bx+8], dx				 ;ЧИСЛО СЕКТОРОВ В таблице
	
	;начало расчета расположения корневого каталога
	xor cx, cx
	
	lea si, boot						;СЧИТАЕМ НАЧАЛО БУТА
	mov cl, [si+5]						;ЧИТАЕМ В CL ЧИСЛО ФАТ ТАБЛИЦ
	mov ax, [si+8]						;ЧИТАЕМ В AX РАЗМЕР ФАТ ТАБЛИЦЫ В СЕКТОРАХ		
	mul cx								;получаем сколько занимают таблицы
	mov dx, [si+3]						;число зарезервированных секторов
	add ax, dx							;тут должны получить адрескорневого каталога
	
	;помещаем адрес корневого каталога в память
	lea bx, root						
	mov word ptr [bx], 	 ax
	mov word ptr [bx+2], 0
	
	;чтение корневого каталога
	lea bx, pack
	mov word ptr [bx], ax						;МЛАДШИЕ РАЗРЯДЫ НОМЕРА СЕКТОРА
	mov word ptr [bx+2], 0						;СТАРШИЕ РАЗРЯДЫ НОМЕРА СЕКТОРА
	mov word ptr [bx+4], 8						;ЧИСЛО СЕКТОРОВ
	mov word ptr [bx+6], offset buf				;СМЕЩЕНИЕ НА БУФЕР
	mov word ptr [bx+8], seg buf				;СЕГМЕНТ БУФЕРА
	mov ax, 0003h
	mov cx, 0FFFFh
	int 25h
	pop ax;
	ret											;В конце выполнения процедуры по адресу buf находится сектор корневого каталога
init endp

finddirs proc near					;Процедурв поиска расположения директорий
	lea di, buf							;идем к загруженному сектору в памяти. найдем первый каталог
	mov bx, di

retry:				;Конструкция до retry выполняет сравнение строк заданного в программе каталога 		
                            ;и имени каталога в корневом каталоге. Как только найдет, переход на equal
	lea si, folder
	mov cx, tmpcx
	jmp LP
fn:
	pop ax
	call fatnext	
LP:	
	MOV al, byte ptr [si] 			;Считывание байта первой строки
	mov dl, byte ptr [di]			;второй
	CMP al, dl						;Сравнение
	JNE notequal					;переход, если строки разные
	inc si							;переход к следующему символу
	inc di							;переход к следующему символу
	dec cx							;dec счётчика
	cmp cx, 0						;Счётчик 0?
	JNE LP		;Переход, если не 0
	jmp equal
notequal:
	add bx, 32
	push ax
	lea ax, pathbuf   ;костыль для проверки на переход в следующий кластер
	dec ax			;pathbuf лежит сразу после buf, поэтому так делать можно
	cmp bx, ax		;
	jg fn			;
	pop ax			;
	mov di, bx
	jmp retry
	
equal:
	mov di, bx			
	
	mov dx, [di+1Ah]	;номер кластера второго каталога
	lea bx, fol
	mov [bx], dx
	lea bx, folold
	mov [bx], dx
					;В начале происходит вычисление. Из номера кластера вычитается двойка, умножается на число секторов в кластере. 
					;К результату необходимо добавить смещение системной области
	xor ax, ax
	xor cx, cx
	lea si, fol
	mov ax, [si]
	sub ax, 2
	lea si, boot		
	mov cl, [si+2]	;sectors per cluster
	mul cx
	push ax		;пусть ax пока полежит подождет
	
	mov ax, [si+6]	;root entries
	mov cx, 32
	mul cx
	mov cx, [si]	;bytes per sectors
	div cx
	
	pop dx
	add ax, dx		;добавим, что сохраняли. Это был отступ
	lea si, root
	mov bx, [si]
	add ax, bx
	
	;начало каталога вычислено
	;Загрузка каталога
	lea si, tmpsec
	mov [si], ax
	
	lea si, pack
	mov word ptr [si], ax
	mov word ptr [si+2], 0
	mov word ptr [si+4], 8	
	mov word ptr [si+6], offset buf
	mov word ptr [si+8], seg buf
	mov bx, si
	mov CX, 0FFFFh
	mov ax, 0003h
	int 25h
	pop ax
	
	;чтение каталога
	lea di, buf
	ret					;в ячейках памяти теперь лежат номера секторов первого и второго каталогов	
finddirs endp

Main:	mov ax, @data
		mov ds, ax
		mov es, ax
		mov es, ax
		call init			;Получение корневого каталога

		; read current directory
		mov ah, 47h
		mov dl, 4
		lea si, pathbuf
		int 21h
		;Если в строке ничего нет, то просто вывод файлов
		;Иначе
		lea di, folder
		mov cx, 0
od_test:
		mov al, BYTE PTR [si]
		cmp al, '\'
		je od_win
		cmp al, 0
		je od_fin ;Если конец пути, то найти адрес последней папки и переход на вывод
		mov BYTE PTR [di], al
		inc si
		inc di
		inc cx
		jmp od_test
od_win:
		inc si
		push si
		mov tmpcx, cx
		call finddirs		;Получение адресов каталогов
		pop si
		mov cx, 0
		lea di, folder
		jmp od_test
od_fin:
		mov tmpcx, cx
		call finddirs
		call output		
		;call findfat
		;call output	
		mov ah, 06h
		int 21h
		mov ah, 4ch
		int 21h

end
