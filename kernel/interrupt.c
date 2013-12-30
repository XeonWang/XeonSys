#define _IRET() __asm__("leave \n\t iret")

#typedef int (*syscall_impl)();

extern int fork_impl();

void clock_interrupt() {
	char *video_memory = (char*) 0xB8000;
	*video_memory = (*video_memory) + 1;
	*video_memory = (*video_memory) + 1;
	__asm__("outb %%al, $0x20"::"a" (0x20));
//	__asm__("outb %%al, $0xA0"::"a" (0x20));
	_IRET();
}

void sys_call_interrupt() {
	__asm__("call %0(,%%eax,4)"::"m" (implements[0]));
}

syscall_impl implements[] = {
	fork_impl
};


