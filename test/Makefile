buildimg: a.img boot.bin
	dd if=boot.bin of=a.img

boot.bin: boot.asm ../util/memcopy.asm
	nasm -I ../util/ -f bin -o boot.bin boot.asm

