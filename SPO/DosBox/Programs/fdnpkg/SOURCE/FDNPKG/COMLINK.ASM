;
; Template program for chaining into another program. This code is not used
; as-is, it's a reference used in FDNPKG for computing COM "link" files.
;
; Copyright (C) 2017 Mateusz Viste
;
; To be compiled with NASM: NASM -f bin test.asm -o test.com
;
; Then, append the full path and filename of the program to chain to, followed
; by a NULL terminator (0).
;

org 100h

; by default the COM file gets all available memory, so I need to free some
; of it so there is enough available for the child program.
mov ah, 4Ah        ; resize memory block at PSP (ES already set to it)
mov bx, 560 >> 4   ; memory needed for the .COM (rounded to a full paragraph):
int 21h            ;     - 256 bytes needed for my PSP
                   ;     - 38 bytes needed for my code and data
                   ;     - up to 256 bytes for the command line

; now set up the pointer to command line arguments in the parameters block
; (offset is pre-set already, but segment still needs to be set)
mov [cmd_seg], ds

; execute the child program now
mov ax, 4B00h      ; load and execute program
mov dx, PROGNAME   ; ASCIIZ path of program
mov bx, params     ; parameter block (at ES:BX)
int 21h

; exit (this) program
int 20h

params:
environ dw 0       ; segment of environment, 0 means to use parent's one
cmd_off dw 80h     ; ptr to command line's offset
cmd_seg dw 0       ; ptr to command line's segment
deffcb1 dd 0       ; pointers to file control blocks
deffcb2 dd 0

PROGNAME:
; nothing here now - once compiled, a program's full path should be appended
; here, followed by a single 0 byte.
