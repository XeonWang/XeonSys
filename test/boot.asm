org 0x7c00

mov esi, 0x7c00
mov edi, 0x8000
mov cx, 5
call memcopy

jmp $

%include 'memcopy.asm'

times 510 - ($ - $$) db 0
  dw 0xaa55
