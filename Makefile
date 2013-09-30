buildimg: boot/boot.bin boot/loader.bin a.img
	dd if=boot/boot.bin of=a.img bs=512 count=1 conv=notrunc
	mkdir -p tempfloppy
	mount -o loop a.img tempfloppy/
	cp boot/loader.bin tempfloppy/ 
	umount tempfloppy/
  
boot/boot.bin: boot/boot.asm boot/include/fat12hdr.inc boot/include/readsector.asm boot/include/comparestr.asm
	nasm -I boot/include/ -o boot/boot.bin boot/boot.asm

boot/loader.bin: boot/loader.asm
	nasm boot/loader.asm -o boot/loader.bin
