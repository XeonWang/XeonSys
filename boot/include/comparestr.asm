;compare cx charactor between ds:si and di

CompareStr:
  push ax
.Begin:
  cmp cx, 0
  je .Equal
  lodsb
  cmp al, [di]
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
