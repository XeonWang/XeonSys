
#define _IRET() __asm__("leave \n\t iret")

extern int fork_impl();
typedef int (*syscall_impl)();

syscall_impl implements[] = {
    fork_impl
};

void clock_interrupt() {
	char *video_memory = (char*) 0xB8000;
	*video_memory = (*video_memory) + 1;
	*video_memory = (*video_memory) + 1;
	__asm__("outb %%al, $0x20"::"a" (0x20));
//	__asm__("outb %%al, $0xA0"::"a" (0x20));
	_IRET();
}

void sys_call_interrupt() {
    __asm__("pushl %eax \n\t pushl %ebx");
    implements[0] = fork_impl;
    __asm__("popl %%ebx \n\t popl %%eax \n\t" \
            "shl $5, %%eax \n\t" \
            "addl %%ecx, %%eax \n\t" \
            "call %%eax"::"c" (implements[0]));
    _IRET();
}
