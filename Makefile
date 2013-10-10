buildimg: clear boot/boot.bin boot/loader.bin a.img boot/kernel.bin
	dd if=boot/boot.bin of=a.img bs=512 count=1 conv=notrunc
	mkdir -p tempfloppy
	mount -o loop a.img tempfloppy/
	cp boot/loader.bin tempfloppy/ 
	cp boot/kernel.bin tempfloppy/
	umount tempfloppy/
  
boot/boot.bin: boot/boot.asm boot/include/fat12hdr.inc boot/include/readsector.asm boot/include/comparestr.asm
	nasm -I boot/include/ -o boot/boot.bin boot/boot.asm

boot/loader.bin: boot/loader.asm
	nasm -I boot/include/ boot/loader.asm -o boot/loader.bin

boot/kernel.bin: boot/kernel.asm
	nasm -f elf boot/kernel.asm -o boot/kernel.o
	ld -s -o boot/kernel.bin boot/kernel.o

clear:
	rm -f boot/*.bin
