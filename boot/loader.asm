org 0x9200

jmp L_START

%include 'fat12hdr.inc'
%include 'gdt.inc'

;GDT: Descriptor 0, 0, 0
;CodeSegmentGDT: Descriptor 0x00000, 0xFFFFF, 0x4000 | 0x94 | 0x8000
;DataSegmentGDT: Descriptor 0, 0xFFFFF, 0x92 | 0x4000 | 0x8000
;VideoSegmentGDT: Descriptor 0xB8000, 0xFFFF, 0x92 | 0x60

;GDTLen equ $ - GDT
;GDTPtr dw GDTLen - 1
;	dd 0x9200 + GDT

;CodeSegmentSel equ CodeSegmentGDT - GDT
;DataSegmentSel equ DataSegmentGDT - GDT
;VideoSegmentSel equ VideoSegmentGDT - GDT

LABEL_GDT:			Descriptor             0,                    0, 0						; 空描述符
LABEL_DESC_FLAT_C:		Descriptor             0x31000,              0fffffh, DA_CR  | DA_32 | DA_LIMIT_4K			; 0 ~ 4G
LABEL_DESC_FLAT_RW:		Descriptor             0,              0fffffh, DA_DRW | DA_32 | DA_LIMIT_4K			; 0 ~ 4G
LABEL_DESC_VIDEO:		Descriptor	 0B8000h,               0ffffh, DA_DRW                         | DA_DPL3	; 显存首地址
; GDT ------------------------------------------------------------------------------------------------------------------------------------------------------------

GdtLen		equ	$ - LABEL_GDT
GdtPtr		dw	GdtLen - 1				; 段界限
		dd	LABEL_GDT		; 基地址

; GDT 选择子 ----------------------------------------------------------------------------------
SelectorFlatC		equ	LABEL_DESC_FLAT_C	- LABEL_GDT
SelectorFlatRW		equ	LABEL_DESC_FLAT_RW	- LABEL_GDT
SelectorVideo		equ	LABEL_DESC_VIDEO	- LABEL_GDT + SA_RPL3
; GDT 选择子 ----------------------------------------------------------------------------------

BaseOfFATs equ 0x0800
BaseOfKernel equ 0x0200
SegOfKernel equ 0x8000
SegOfRunableKernel equ 0x3000
BaseOfRunableKernel equ 0x1000

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
  and ax, 0x0FFF
.ReadNextKernelSector:
  cmp ax, 0xFF7
  jg .KernelLoaded
  pop bx
  add bx, word [BPB_BytsPerSec] 
  jmp .ReadKernelSector

.KernelLoaded:
  mov ax, 0
  call relocateKernel
  
  mov ax, 0x0 
;  mov cs, ax
  mov ds, ax
  mov es, ax
  mov ss, ax
  mov gs, ax

  
  lgdt [GdtPtr]
  cli
  in al, 0x92
  or al, 00000010b
  out 0x92, al

  mov eax, cr0
  or eax, 1
  mov cr0, eax

  mov ax, SelectorVideo
  mov gs, ax
;jmp CodeSegmentSel:BaseOfRunableKernel + 0x400
  ;jmp dword SelectorFlatC:0x9200+PM_START
  jmp dword SelectorFlatC:0x400

  jmp $

.FileNotFoundInRoot:
  jmp $



relocateKernel:
  mov ax, SegOfKernel
  mov ds, ax
  xor edx, edx
  mov dx, word [ds:BaseOfKernel + 0x2C]
  mov eax, dword [ds:BaseOfKernel + 0x1C]
.segmentloop: 
  cmp edx, 0
  je .return
  mov ebx, dword [ds:BaseOfKernel + eax + 4]
  mov ecx, dword [ds:BaseOfKernel + eax + 8]
  
  xor esi, esi
  xor edi, edi
  mov esi, ebx 
  add esi, BaseOfKernel
  mov edi, ecx
  add edi, BaseOfRunableKernel
  mov ecx, dword [ds:BaseOfKernel + eax + 16] 
  call memcopy
  dec edx
  add eax, 0x20
  jmp .segmentloop 
.return:
  ret

  %include 'readsector.asm'
  %include 'comparestr.asm'
  %include 'memcopy.asm'

KernelName db 'KERNEL  BIN'
