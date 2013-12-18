MOV AH, 0x0E

MOV AL, 'H'
int 0x10
MOV AL, 'e'
int 0x10
MOV AL, 'l'
int 0x10
MOV AL, 'l'
int 0x10
MOV AL, 'o'
int 0x10

JMP $

times 510-($-$$) DB 0
DW 0xAA55
