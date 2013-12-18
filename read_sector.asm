ReadSector:
	push	bp
	mov	bp, sp
	sub	esp, 2

	mov	byte [bp-2], cl
	push	bx
	mov	bl, 18
	div	bl
	inc	ah
	mov	cl, ah
	mov	dh, al
	shr	al, 1	
	mov	ch, al
	and	dh, 1	
	pop	bx
	mov	dl, 0
.GoOnReading:
	mov	ah, 2
	mov	al, byte [bp-2]	
	int	13h
	jc	.GoOnReading

	add	esp, 2
	pop	bp

	ret
