org 0x7c00

jmp L_START
nop

%include 'fat12hdr.inc'

BaseOfStack equ 0x7c00
BaseOfFATs equ 0x8000
BaseOfLoader equ 0x9200

L_START:
  mov ax, cs
  mov ds, ax
  mov es, ax
  mov ss, ax
  mov sp, BaseOfStack

  mov ax, 0x0600
  mov bx, 0x0700
  mov cx, 0x0
  mov dx, 0x184F
  int 10

  mov dx, 0
.SearchInRoot: 
  cmp dx, RootSectorNum
  je .FileNotFoundInRoot
  push dx
  mov ax, dx
  add ax, BeginOfRootSector
  mov cl, 1
  mov bx, 0x7E00
  call ReadSector
  ;find loader from 0x7E00-0x8100
  mov cx, 0
.SearchInSector:
  cmp cx, RootEntCntPerSector
  je .FileNotFoundInSector
  ;check file name in one root entity
  mov ax, cx
  push cx
  mov cx, LengthOfFileName
  mov bx, LengthOfRootEnt
  mul bl ;32
  add ax, 0x7E00
  mov si, ax 
  mov di, LoaderName
  call CompareStr
  cmp cx, 1
  je .FileFound
  pop cx
  inc cx
  jmp .SearchInSector
.FileNotFoundInSector:
  inc dx
  jmp .SearchInRoot 
.FileFound:
  push ax
  mov ax, 1
  mov bx, BaseOfFATs
  mov cl, 9
  call ReadSector
  pop ax

  add ax, 26
  mov bx, ax
  mov ax, [bx]
  xor bx, bx
.ReadLoaderSector:
  push ax
  add ax, 31  ; 31 = BeginOfRootSector + RootSectorNum - 2
  mov cl, 1
  add bx, BaseOfLoader
  call ReadSector
  pop ax
  mov bx, 3
  mul bx
  mov bx, 2
  div bx
  cmp dx, 0
  je .ByteBegin
  add ax, BaseOfFATs
  mov bx, ax
  mov ax, [bx]
  shr ax, 4
  jmp .ReadNextLoaderSector
.ByteBegin:
  add ax, BaseOfFATs
  mov bx, ax
  mov ax, [bx]
  and ax, 0x0FFF
.ReadNextLoaderSector:
  cmp ax, 0xFF7
  jg .LoaderLoaded
  pop bx
  inc bx
  jmp .ReadLoaderSector

.LoaderLoaded:
  mov ax, 0
  jmp BaseOfLoader

  jmp $
.FileNotFoundInRoot:
  jmp $

  %include 'readsector.asm'
  %include 'comparestr.asm'

LoaderName db 'LOADER  BIN'

times 510 - ($-$$) db 0
  dw 0xAA55
