[BITS 32]
[extern main]
	MOV AX, 0x10
	MOV DS, AX
	MOV ES, AX
	MOV FS, AX

	CALL main
	JMP $
