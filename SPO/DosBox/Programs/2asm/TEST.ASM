; this is a *weird* listing file containing instructions which caused the
; disassembler grief at one time or another.  They all seem to work now.

.386
.radix 16
; code segment use32
code segment use16
       
         org 100h
start:

   rep lock test dword ptr es:[12345678], 87654321 ; 13 bytes
                                          ; screws up CodeView <ha ha ha>
   db 067h, 08bh, 0c1h
   db 0,0
   db 066,0,0             ; bug in TD ? yup!
   mov ax, [bx]
   mov ax, [ebx]
   mov ax, [edi]
   mov ax, [bp+1]
   mov eax, [bx]
   mov eax, [ebx]
   mov eax, [edi]
   mov eax, [bp+1]
   push 12              ; sign extend

   push 0ca             ; sign extend
   push 0cafe           ; no sign extend
   push 12345678
   db 0f6, 0c4, 08      ; test ah, 08
   test dword ptr ds:[100], 12345678
   test word ptr es:[100], 1
   test byte ptr es:[100], 1
   test word ptr ds:[100], 1     ; !
   test byte ptr ds:[100], 1
   test ax, 1
   test al, 1
   test eax, 1
   test di, 80
   test di, 0ff80          ; sign extension
; jecxz $-10
; jcxz $+10
; jmp short $+1
 db 0f7, 0c7, 080, 00       ; oddball instructions
 db 0f7, 046, 04, 40, 00
 db 066, 0e3, 090
 db 0e3, 090
 db 084, 10, 90
 db 085, 10, 90
  db 83, 0f9, 02
  db 83, 0f9, 0ca
  db 20, 43, 6f
  db 06a, 0ca
  db 082, 047, 10, 20
  test [bx-1f], ebp
  db 8bh, 07
  db 67, 8bh, 07
  db 068, 0ca, 0, 068, 0fe, 99
  db 066h, 00, 00
  db 01, 00
  db 066h, 01, 00
;  jecxz $+2
  idiv dh
  idiv si
  db 06bh, 0c0h, 0d4h
  db 06ah, 0c7h
  db 0f, 020, 014
  db 08e, 03e, 0, 0
  db 08c, 0cbh

  db 0f6, 0f9
  db 0f7, 0f9
  db 0f6, 0f8
  db 0f7, 0f8
  db 0d0, 0f0
  db 0d3, 0f3
  db 0e8h, 0ebh, 0ebh
  db 066, 020, 04d, 061
  db 066, 020, 035
  db 081, 03f, 0, 1
  db 0fe, 07
 db 080, 03e, 00, 00, 05a
  db 0f6, 0b1, 0f1, 0af
  db 026, 01, 06, 085, 0e
  db 0c7, 06, 3, 0, 0ff, 0
 db 08e, 05e, 0
  db 26h, 89h, 0fh
  db 08bh, 037h
  db 08bh, 00Eh, 0d2h, 01h
  db 3, 6, 3, 0

end start
code ends
end
