#ifndef _REGISTER_
#define _REGISTER_

#define add_to_gdt(index, limit, addr, type) { \
	__asm__("movw %%bx, (%%eax)\n\t" \
		"addl $2, %%eax\n\t" \
		"movw %%cx, (%%eax)\n\t" \
		"addl $2, %%eax\n\t" \
		"rorl $16, %%ecx\n\t" \
		"movb %%cl, (%%eax)\n\t" \
		"addl $1, %%eax\n\t" \
		"movb %%dl, (%%eax)\n\t" \
		"rorl $16, %%ebx\n\t" \
		"andb $0x0F, %%bl\n\t" \
		"addl $1, %%eax\n\t" \
		"movb %%bl, (%%eax)\n\t" \
		"addl $1, %%eax\n\t" \
		"movb %%ch, (%%eax)\n\t" \
		::"a" (index), "b" (limit), "c" (addr), "d" (type) \
	); \
}

#define change_to_user_mode() { \
	__asm__("movl %%esp, %%eax \n\t" \
		"pushl $0x17 \n\t " \
		"pushl %%eax \n\t " \
		"pushfl \n\t " \
		"pushl $0x0f \n\t " \
		"pushl $1f \n\t " \
		"iret \n\t" \
		"1: \n\t" \
		"movl $0x17, %%eax \n\t" \
		"movw %%ax, %%ds\n\t" \
		"movw %%ax, %%es\n\t" \
		"movw %%ax, %%fs\n\t" \
		"movw %%ax, %%gs\n\t" \
		:::"ax"); \
}

#define _STI() __asm__("sti");

#endif
