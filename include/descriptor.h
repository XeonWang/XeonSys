#ifndef _DESC_H
#define _DESC_H

#define _LGDT(gdt) __asm__( \
	"lgdt gdt_desc\n\t" \
	"gdt_desc:\n\t" \
	".word 256*8-1 \n\t"\
	".long %0" \
	::"m" (*gdt) \
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
