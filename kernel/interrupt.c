void clock_interrupt() {
	char *video_memory = (char*) 0xB8000;
	*video_memory = (*video_memory) + 1;
	__asm__("movb $0x20, %al \n\t outb %al, $0x20");
}
