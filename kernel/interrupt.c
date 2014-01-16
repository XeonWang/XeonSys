#include <descriptor.h>

#define _IRET() __asm__("leave \n\t iret")

extern struct interrupt_desc idt[];

extern int fork_impl();
typedef int (*syscall_impl)();

syscall_impl implements[] = {
    fork_impl
};

void clock_interrupt() {
    __asm__("movw %ax, %ax");
//	char *video_memory = (char*) 0xB8000;
//	*video_memory = (*video_memory) + 1;
//	*video_memory = (*video_memory) + 1;
//	__asm__("outb %%al, $0x20"::"a" (0x20));
////	__asm__("outb %%al, $0xA0"::"a" (0x20));
    schedule();
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

void set_interrupt(int index, void (*addr)(), unsigned short seg_selector, unsigned short dpl) {
    __asm__("movw %ax, %ax");
    idt[index].addr_0_15 = (unsigned short)addr;
    idt[index].seg_selector = seg_selector;
    idt[index].type = 0x8E00+(dpl<<13);
    idt[index].addr_16_31 = (unsigned short)((unsigned)addr>>16);
}
