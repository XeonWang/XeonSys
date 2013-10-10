;compare cx charactor between ds:si and di

CompareStr:
  push ax
  mov cx, 11
.Begin:
  cmp cx, 0
  je .Equal
  push ds
  mov ax, 0
  mov ds, ax
  lodsb
  pop ds
  cmp al, [cs:di]
  jne .NotEqual
  inc di
  loop .Begin
.Equal:
  mov cx, 1
  jmp .ret
.NotEqual:
  mov cx, 0
  jmp .ret

.ret:
  pop ax
  ret
