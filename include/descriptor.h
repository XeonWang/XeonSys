#ifndef _DESC_H
#define _DESC_H

#define _LGDT(descriptor) __asm__( \
	"lgdt %0\n\t" \
	::"m" (descriptor) \
)

#define _LIDT(descriptor) __asm__( \
	"lidt %0\n\t" \
	::"m" (descriptor) \
)

#define _LTR(selector) __asm__( \
	"ltr %%ax\n\t" \
	::"a" (selector) \
)

#define _LLDT(selector) __asm__( \
	"lldt %%ax\n\t" \
	::"a" (selector) \
)

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

/*
#define set_interrupt(index, addr, seg_selector) (\
	idt[index].addr_0_15 = (unsigned short)addr; \
	idt[index].seg_selector = seg_selector; \
	idt[index].type = 0x8E00; \
	idt[index].addr_16_31 = (unsigned short)(addr>>16); \
)
*/
#endif

struct global_desc {
	char limit_0_7, limit_8_15;
	char base_0_7, base_8_15;
	char base_16_23;
	char type; //this type include type and P, DPL...
	char limit_16_19; //this field include G, AVL, and limit bit 16-19...
	char base_24_31; 
};

struct interrupt_desc {
	short addr_0_15;
	short seg_selector;
	short type;
	short addr_16_31;	
};

#pragma pack (2)
struct desc_desc {
	short limit;
	unsigned long addr;
};
#pragma pack ()
