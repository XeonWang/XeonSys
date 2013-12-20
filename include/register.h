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

#endif
