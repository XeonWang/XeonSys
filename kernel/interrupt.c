#define _IRET() __asm__("leave \n\t iret")
void clock_interrupt() {
	char *video_memory = (char*) 0xB8000;
	*video_memory = (*video_memory) + 1;
	*video_memory = (*video_memory) + 1;
	__asm__("outb %%al, $0x20"::"a" (0x20));
//	__asm__("outb %%al, $0xA0"::"a" (0x20));
	_IRET();
}
