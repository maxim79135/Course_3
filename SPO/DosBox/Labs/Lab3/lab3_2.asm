.MODEL small
.DATA
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
Packet STRUC							
	sec			db 4 dup(0)	 			
	number		db 2 dup(0)	 	
	transfer	db 4 dup(0)				
Packet ENDS

bootsec STRUC							
	sectorsize	db 2  dup(0)	;Размер сектора   		
	sectors		db 1  dup(0)    ;Количество секторов в кластере
	reserved	db 2  dup(0)    ;Количество зарезервированных секторов		
	fatnum		db 1  dup(0)    ;Количество фат таблиц
	rootentr	db 2  dup(0)    ;Количество записей корневого каталога		
	fatsecs		db 2  dup(0)    ;Количество секторов в фат таблице
bootsec ENDS

;имена каталогов и файла
folder db	'        '					
pack Packet 1 dup(<>)					
fol	dd 4								

;Загрузочный сектор
boot bootsec 1 dup(<>)					
root dd 4								
tmpcx dw 2								
tmpsec dd 4								

;Переменная для кластера
buf db 4096 dup(?)
	db '$'								

;Буфер для корневого каталога
bufroot db 19968 dup(?)
	db '$'								

;Переменная для пути к каталогу
pathbuf db 64 dup('\'), 0				
pathbufroot db 64 dup('\'), 0

system dd 4								

fatbuf db 512 dup(?)
	db '$'								 

tempproverenzap dw 2
tempprovzap dw 2

;Флаги для условий
flagroot db 1 dup(1)
countzaproot dw 2 dup(0)
errordir db 1 dup(0)
endclasterfff db 1 dup(0)

endsysobl dd 4
mesdirectory db 0Ah, 0Dh, 'Enter directory: $'

strbuf db 64 ;путь каталога

.stack 256h

.CODE

jmp MAIN

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;Вывод сообщений
MESSAGE proc near
        push ax
        mov ah, 09h
        int 21h
        pop ax
        ret
MESSAGE endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;Процедура считывания следующего кластера
FAT_NEXT proc near
	push si 							
	push di 							
	push cx 							

	lea si, fol 						;number current claster					
	mov ax, word ptr [si]
	mov cx, 2
	mul cx
	mov si, ax

	push si
	lea si, boot 						;Считываем бут сектор
	mov di, word ptr [si+3]
	pop si

SEARCH_FAT_NEXT:
	lea bx, pack						
	mov word ptr [bx], di				
	mov word ptr [bx+2], 0				
	mov word ptr [bx+4], 1				
	mov word ptr [bx+6], offset fatbuf	;Считывыаем таблицу фат
	mov word ptr [bx+8], seg fatbuf		
	mov cx, 0FFFFh						
	mov al, 03h							
	int 25h								
	pop ax 								
	
	inc di 								
	cmp si, 512							;Запись хранится в текущем сегменте таблицы фат?
	jl READ_CLASTER						
	jmp NEXT_FAT_SECTOR

NEXT_FAT_SECTOR:
	SUB si, 512
	jmp SEARCH_FAT_NEXT

READ_CLASTER:							
	lea di, fatbuf
	mov bx, di 							;Читаем таблицу фат
	mov cx, word ptr [bx+si]
	cmp cx, 0FFFFh
	je IF_FFFF 							;Если этот кластер последний то не считываем его, иначе читаем
	lea si, fol
	mov word ptr [si], cx

;Расчитываем путь кластера и считываем его
FOUND_SECTOR:
	xor ax, ax 					
	xor cx, cx 							
	lea si, fol 						
	mov ax, word ptr [si] 				
	SUB ax, 2 							
	lea si, boot						
	mov cl, byte ptr [si+2]				
	mul cx 								
	
	lea di, system
	mov dx, word ptr [di]
	add ax, dx 							
	
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
	jmp END_FAT_NEXT

IF_FFFF:	
	push si
	lea si, endclasterfff
	mov byte ptr[si], 1
	pop si

END_FAT_NEXT:
	pop cx 								
	pop di 								
	pop si								

	RET
FAT_NEXT endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;Процедура вывода файлов на экран
OUTPUT proc near						
	push si
	push di
	push ax
	push cx
	push dx
	xor dx, dx
	lea si, tempprovzap
	mov byte ptr [si], 0
	lea di, boot
	mov ax, word ptr [di]
	mov cx, 32
	div cx
	xor cx, cx
	mov cl, byte ptr [di+2]
	mul cx
	lea si, tempproverenzap			;Количество записей в кластере
	mov [si], ax
	pop dx
	pop cx
	pop ax
	pop di
	pop si

	push si
	lea si, flagroot
	cmp byte ptr [si], 1
	jne READ_BUF_OUT
	lea di, bufroot							
	mov bx, di 
	pop si							
	jmp CLEAR_SREEN

READ_BUF_OUT:
	lea di, buf
	mov dx, di
	pop si

CLEAR_SREEN:
	mov ax, 0
	push ax
	jmp NEXT_FILE

PRINT_NEXT_KLAST:
	lea di, buf							
	jmp NEXT_FILE

NEW_SCREEN:
	mov tmpcx, 0						
	mov AX, 3h   						
    int     10h     					

NEXT_FILE:
	mov cx, tmpcx 							
	cmp cx, 25							
	je NEW_SCREEN 							
	mov ax, [di]						
	cmp ax, 0h 			;Если запись содержит первый байт 0, то идем на выход					
	je E_N_D							
	mov al, [di]						
	cmp al, 0e5h 		;Пропускаем файл если он удален				
	je SKIP_FILE						

	;Сравниваем расширение из буфера на COM
	mov dl, byte PTR [di+8]
	cmp dl, 'C'
	jne SKIP_FILE
	mov dl, byte PTR [di+9]
	cmp dl, 'O'
	jne SKIP_FILE
	mov dl, byte PTR [di+10]
	cmp dl, 'M'
	jne SKIP_FILE

	pop ax
	cmp ax, 0
	jne NEXT_COLOR 
	mov bx, 01
	mov ax, 1
	push ax
	jmp OUT_FILE

NEXT_COLOR:
	mov bx, 02
	mov ax, 0
	push ax

OUT_FILE:
    mov al, 1    						
    mov cx, 11							
	lea si, tmpcx 						
    mov dh, [si]    					
    mov ah, 13h  						
    mov dl, 0    						
	mov bp, di 							
    int 10h     						
	mov ah, 10h  						
    int 16h     						
	inc tmpcx 							

SKIP_FILE:
	
	push si
	lea si, flagroot
	cmp byte ptr [si], 1
	pop si	
	je SKIP_FILE_IF_ROOT 
	
SKIP_FILE_IF_BUF:
	push si
	push cx
	lea si, tempprovzap
	mov cx, word ptr [si]  
	cmp cx, word ptr tempproverenzap ;Если количество проверенных записей равно колич записей в кластере, то переходим на след кластер
	jne NEXT_ZAPIS
	call FAT_NEXT
	lea si, tempprovzap
	mov word ptr [si], 0
	pop cx
	pop si
	jmp PRINT_NEXT_KLAST

SKIP_FILE_IF_ROOT:
	push si
	push cx
	lea si, tempprovzap
	mov cx, word ptr [si]  
	cmp cx, word ptr countzaproot
	jne NEXT_ZAPIS
	pop cx
	pop si
	jmp E_N_D

NEXT_ZAPIS:
	inc cx
	mov word ptr [si], cx
	pop cx
	pop si
	add di, 32						; +32 переходим на следующую запись			
	jmp NEXT_FILE

E_N_D:			
	pop ax					

	RET
OUTPUT endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Процедура считывания корневого каталога
INIT proc near 							
	mov al, 03h							
	lea bx, pack						
	mov word ptr [bx], 0				
	mov word ptr [bx+2], 0				
	mov word ptr [bx+4], 8				
	mov word ptr [bx+6], offset bufroot	 ;Читаем бут сектор
	mov word ptr [bx+8], seg bufroot		
	mov cx, 0FFFFh						
	int 25h								
	pop ax 								
	
	lea si, bufroot						
	lea bx, boot						
	mov dx, [si+0Bh] 					
	mov word ptr [bx], dx				;размер сектора в байтах
	mov dx, [si+0Dh]					
	mov byte ptr [bx+2], dl             ;размер кластера в секторах
	mov dx, [si+0Eh]					
	mov word ptr [bx+3], dx             ;число зараезервированных секторов
	mov dx, [si+10h]					
	mov byte ptr [bx+5], dl             ;число FAT таблиц
	mov dx, [si+11h]						
	mov word ptr [bx+6], dx             ;число записей в корневом каталоге
	mov dx, [si+16h]					
	mov word ptr [bx+8], dx				;число секторов таблице
	
	;начало расчета расположения корневого каталога
	xor cx, cx 							
	lea si, boot						
	mov cl, [si+5]						
	mov ax, [si+8]						
	mul cx								
	mov dx, [si+3]						
	add ax, dx							

	lea bx, countzaproot
	mov cx, word ptr [si+6]
	mov [bx], cx

	lea bx, root						
	mov word ptr [bx], ax 				
	mov word ptr [bx+2], 0 				


	;расчет размера системной области
	push ax
	lea si, boot
	mov ax, word ptr [si+6]
	mov di, 32
	mul di
	mov di, [si]						;512
	div di
	lea si, root
	mov dx, word ptr [si]
	add ax, dx
	lea si, system
	mov word ptr [si], ax
	mov word ptr [si+2], 0
	pop ax

	lea bx, pack 						
	mov word ptr [bx], ax				
	mov word ptr [bx+2], 0				
	mov word ptr [bx+4], 39				
	mov word ptr [bx+6], offset bufroot	
	mov word ptr [bx+8], seg bufroot	 ;Считываем корневой каталог
	mov ax, 0003h 						
	mov cx, 0FFFFh 						
	int 25h 							
	pop ax 								
	RET									
INIT endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;Находим путь к веденному пути
FIND_DIR proc near						
	push si
	push di
	push ax
	push cx
	push dx
	xor dx, dx
	lea si, tempprovzap
	mov [si], 0
	lea di, boot
	mov ax, word ptr [di]
	mov cx, 32
	div cx
	xor cx, cx
	mov cl, byte ptr [di+2]
	mul cx
	lea si, tempproverenzap 	;записей в кластере
	mov [si], ax
	pop dx
	pop cx
	pop ax
	pop di
	pop si

	push si
	lea si, flagroot
	cmp byte ptr [si], 1
	jne READ_BUF
	lea di, bufroot							
	mov bx, di 
	pop si							
	jmp RETRY

READ_BUF:
	lea di, buf
	mov dx, di
	pop si

;ТУТ ПРОИСХОДИТ ПОИСК КЛАСТЕРА

RETRY:				
	lea si, folder 						
	mov cx, tmpcx 						
	jmp LP 								
	
NEXT_FAT:
	;pop ax 							
	call FAT_NEXT						
	push si
	lea si, tempprovzap
	mov [si], 0
	lea si, endclasterfff
	cmp byte ptr [si], 1
	pop si
	pop cx
	pop si
	je PATH_ERROR_BUF

LP:	
	mov al, byte ptr [si] 				
	mov dl, byte ptr [di]				
	cmp al, dl							
	jne IF_NOT_EQUAL					
	inc si								
	inc di								
	dec cx								
	cmp cx, 0							
	jne LP								
	jmp IF_EQUAL						

IF_NOT_EQUAL:
	push si
	lea si, flagroot
	cmp byte ptr [si], 1
	pop si
	je ROOT_ENTRIES

;ЕСЛИ БУФЕР 
	push si
	push cx
	lea si, tempprovzap
	mov cx, word ptr [si]  
	cmp cx, word ptr tempproverenzap  ;Если количество записей равно колич записей в кластере, то следующий кластер считываем, иначе читаем следующую запись
	je NEXT_FAT
	inc cx
	mov word ptr [si], cx
	pop cx
	pop si
	jmp NEXT_ENTRIES

ROOT_ENTRIES:
	push si
	push cx
	lea si, tempprovzap
	mov cx, word ptr [si]
	cmp cx, word ptr countzaproot
	je PATH_ERROR_ROOT
	inc  cx
	mov word ptr [si], cx
	pop cx
	pop si
	jmp NEXT_ENTRIES

NEXT_ENTRIES:
	add bx, 32      					
	mov di, bx  						
	jmp RETRY							

;; ТУТ ОНО ЗАКАНЧИВАЕТСЯ
;Если путь найден считываем начальный кластер и загружаем его в буфер
IF_EQUAL:
	mov di, bx							
	mov dx, [di+1Ah]					
	lea bx, fol 						
	mov [bx], dx 						

	xor ax, ax 							
	xor cx, cx 							
	lea si, fol 						
	mov ax, [si] 						
	SUB ax, 2							
	lea si, boot						
	mov cl, [si+2]						
	mul cx 									
	push ax								
	
	mov ax, [si+6]						;число зап в корневом катал
	mov cx, 32 							
	mul cx 								
	mov cx, [si]						;размер одного сектора
	div cx 								
	
	pop dx 								
	add ax, dx							
	lea si, root 						
	mov bx, [si] 						
	add ax, bx 							
	

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
	
	lea bx, flagroot
	mov word ptr [bx], 0

	lea di, buf 						
	jmp END_FIND_DIR


PATH_ERROR_BUF:
	lea si, errordir
	mov byte ptr [si], 1
	jmp END_FIND_DIR

PATH_ERROR_ROOT:
	lea si, errordir
	mov byte ptr [si], 1
	pop cx
	pop si

END_FIND_DIR:
	RET										
FIND_DIR endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

MAIN:	
	mov ax, @data
		mov ds, ax
		mov es, ax

		call INIT						;Считываем корневой каталог

INPUT_DIR:
		lea bx, flagroot
		mov byte ptr [bx], 1

		lea bx, errordir
		mov byte ptr [bx], 0
		
		mov     AX,3H   
   		int     10H     

		lea dx, mesdirectory
		call MESSAGE
		
		mov DX, offset strbuf				
		
		mov AH, 0Ah						
		int 21h					 		;Считываем строку введенную пользователем		

		xor cx, cx
		mov si, offset strbuf			
		mov cl, byte ptr [si+1]	
		mov bx, 2

;ПЕРЕВОДИМ В КАПС
n1:
    	mov al,byte ptr [si+bx]
    	cmp al,60h
    	jl NEXT_SIMBOL
        	sub al,20h
        	mov byte ptr [si+bx], al
NEXT_SIMBOL:
		inc bx       
    	loop n1
;;;
		mov DI, offset strbuf				
		lea si, pathbuf
		xor bx, bx
		xor cx, cx	
		mov cl, byte ptr [DI+1]					
		cmp cx, 0
		je IF_ROOT

READ_STR:		
		mov al, byte ptr [di+bx+2]
		mov [si+bx], al
		inc bx
		loop READ_STR
		inc cx

IF_ROOT:
		mov byte ptr [si+bx], 0
		cmp cx, 0
		je GO_TO_DIRECTORY

;;;;;;;;;;;;;;;

GO_TO_DIRECTORY:
		mov al, [si]
		cmp al, 0
		je OT
		;Если в строке ничего нет, то просто вывод файлов
		;Иначе
		lea di, folder
		mov cx, 0

OD_TEST:
		mov al, BYTE PTR [si]
		cmp al, '\'
		je DIR_NEXT
		cmp al, 0
		je DIR_FIN 						
		mov BYTE PTR [di], al
		inc si
		inc di
		inc cx
		jmp OD_TEST

DIR_NEXT:
		inc si
		push si
		mov tmpcx, cx
		call FIND_DIR		
		lea si, errordir
		cmp byte ptr [si], 1
		je INPUT_DIR
		pop si
		mov cx, 0
		lea di, folder
		jmp OD_TEST

DIR_FIN:
		mov tmpcx, cx
		call FIND_DIR
		push si
		lea si, errordir
		cmp byte ptr [si], 1
		je INPUT_DIR
		pop si
OT:		
		call OUTPUT		
		
		mov ah, 4ch
		int 21h
END
