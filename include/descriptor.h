#ifndef _DESC_H
#define _DESC_H

#define _LGDT(descriptor) __asm__( \
	"lgdt %0\n\t" \
	::"m" (descriptor) \
)

#endif

struct global_desc {
	char limit_0_7, limit_8_15;
	char base_0_7, base_8_15;
	char base_16_23;
	char type; //this type include type and P, DPL...
	char limit_16_19; //this field include G, AVL, and limit bit 16-19...
	char base_24_31; 
};

#pragma pack (2)
struct gdt_desc {
	short limit;
	unsigned long addr;
};
#pragma pack ()
