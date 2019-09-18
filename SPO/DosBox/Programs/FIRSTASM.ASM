.model small
.data
  message0 db 'Hello world! $'
  
.stack 200h
.code
main:
  mov ax, @data
  mov ds, ax
  
  lea dx, message0
  mov ah, 09h
  int 21h
  
  mov ah, 4ch
  int 21h        
end main