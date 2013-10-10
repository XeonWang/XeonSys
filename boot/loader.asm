org 0x9200

jmp L_START

%include 'fat12hdr.inc'

BaseOfFATs equ 0x0800
BaseOfKernel equ 0x0200
SegOfKernel equ 0x8000

L_START:
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
  ;find kernel from 0x7E00-0x8100
  mov cx, 0
.SearchInSector:
  cmp cx, RootEntCntPerSector
  ;check file name in one root entity
  mov ax, cx
  push cx
  mov cx, LengthOfFileName
  mov bx, LengthOfRootEnt
  mul bl ;32
  add ax, 0x7E00
  xor esi, esi
  mov si, ax
  mov di, KernelName
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
  mov ax, [BPB_RsvdSecCnt]
  mov bx, BaseOfFATs
  mov cl, 1
  call ReadSector
  pop ax
  add ax, 26
  mov bx, ax
  mov ax, [cs:bx]
  mov bx, BaseOfKernel
.ReadKernelSector:
  push ax
  add ax, 31 ; 31 = BeginOfRootSecotr + RootSectorNum - 2
  mov cl, 1
  push es
  push ax
  mov ax, SegOfKernel
  mov es, ax
  pop ax
  call ReadSector
  pop es
  pop ax
  push bx
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
  jmp .ReadNextKernelSector
.ByteBegin:
  add ax, BaseOfFATs
  mov bx, ax
  mov ax, [bx]
  add ax, 0x0FFF
.ReadNextKernelSector:
  cmp ax, 0xFF7
  jg .KernelLoaded
  pop bx
  add bx, word [BPB_BytsPerSec] 
  jmp .ReadKernelSector

.KernelLoaded:
  mov ax, 0
  jmp SegOfKernel:BaseOfKernel

  jmp $

.FileNotFoundInRoot:
  jmp $

  %include 'readsector.asm'
  %include 'comparestr.asm'

KernelName db 'KERNEL  BIN'

