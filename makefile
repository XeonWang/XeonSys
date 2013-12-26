image: all.bin
	dd conv=notrunc if=all.bin of=os.img bs=512 count=10

boot: boot.asm
	nasm boot.asm -o boot

loader: init_GDT_IDT.asm
	nasm init_GDT_IDT.asm

kernel.bin: kernel/main.c kernel/task.c entry.asm
	nasm entry.asm -f elf -o entry.o
	gcc -ffreestanding -c kernel/main.c -o kernel/main.o -I include
	gcc -ffreestanding -c kernel/task.c -o kernel/task.o -I include
	gcc -ffreestanding -c kernel/interrupt.c -o kernel/interrupt.o -I include
	ld -o kernel.bin -Ttext 0xB00 entry.o kernel/main.o kernel/task.o kernel/interrupt.o --oformat binary

all.bin: boot loader kernel.bin
	cat boot init_GDT_IDT kernel.bin > all.bin

clean:
	rm -f *.o
	rm -f *.bin
	rm -f kernel/*.o kernel/*.bin

fd: all.bin
	dd conv=notrunc if=all.bin of=/dev/fd0 bs=512 count=10
