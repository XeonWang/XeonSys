image: all.bin
	dd conv=notrunc if=all.bin of=os.img bs=512 count=7

boot: boot.asm
	nasm boot.asm -o boot

loader: init_GDT_IDT.asm
	nasm init_GDT_IDT.asm

kernel.bin: kernel/main.c entry.asm
	nasm entry.asm -f elf -o entry.o
	gcc -ffreestanding -c kernel/main.c -o kernel/main.o -I include
	ld -o kernel.bin -Ttext 0x7E00 entry.o kernel/main.o --oformat binary

all.bin: boot loader kernel.bin
	cat boot init_GDT_IDT kernel.bin > all.bin

clean:
	rm -f *.o
	rm -f *.bin
	rm -f kernel/*.o kernel/*.bin

