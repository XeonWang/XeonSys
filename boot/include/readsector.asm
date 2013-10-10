;Read cl sectors begin with ax to es:bx

ReadSector:
  push dx
  push cx

  div byte [BPB_SecPerTrk]
  push ax
  and ax, 0x0001
  mov dh, al
  pop ax
  shr al, 1
  mov ch, al
  inc ah
  mov cl, ah
  mov dl, 0
  pop ax
  mov ah, 0x2
  int 19

  pop dx
  ret
