ORG 0x7C00

loader_offset equ 0x100

JMP begin

begin:
    MOV AX, 0xB800
    MOV GS, AX
    MOV EDI, 0
    MOV AH, 0x0C
    MOV AL, 'P'
    MOV [GS:EDI], AX

    MOV CX, 0x000C
    MOV BX, loader_offset 
    MOV AX, 1
read_loader:
    PUSH CX
    PUSH AX
    MOV CL, 1
    CALL ReadSector
    POP AX
    POP CX
    ADD BX, 0x200
    INC AX
    LOOP read_loader
;    MOV CX, 0x0006
;    MOV BX, loader_offset 
;read_loader:
;    PUSH CX
;    MOV AH, 0x02
;    MOV AL, 1
;    MOV CH, 0
;    MOV CL, 0x02
;    MOV DH, 0
;    MOV DL, 0
;    INT 0x13
;    POP CX
;    ADD BX, 0x200
;    LOOP read_loader

    JMP loader_offset

%include "read_sector.asm"

;[BITS 32]
;code32:
;    MOV AX, VEDIO_SEG
;    MOV GS, AX
;    MOV EDI, 0
;    MOV AH, 0x0C
;    MOV AL, 'X'
;    MOV [GS:EDI], AX
;
;    JMP $

TIMES 510 -($-$$) DB 0
    DW 0xaa55

