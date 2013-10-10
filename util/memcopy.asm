;Copy cx byte memory from si to di

memcopy:
	push ax
.loop:
	lodsb
	mov [di], al
	inc di
	loop .loop
	pop ax
	
	ret
