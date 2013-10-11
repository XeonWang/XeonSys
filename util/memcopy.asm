;Copy ecx byte memory from ds:si to ds:di

memcopy:
	push ax
	mov ax, 0x3000
	mov es, ax
.loop:
	lodsb
	mov [es:edi], al
	inc di
	loop .loop
	pop ax
	
	ret
