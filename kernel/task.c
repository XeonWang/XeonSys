#include <descriptor.h>

extern GDT;

struct tss_struct {
	long	back_link;	/* 16 high bits zero */
	long	esp0;
	long	ss0;		/* 16 high bits zero */
	long	esp1;
	long	ss1;		/* 16 high bits zero */
	long	esp2;
	long	ss2;		/* 16 high bits zero */
	long	cr3;
	long	eip;
	long	eflags;
	long	eax,ecx,edx,ebx;
	long	esp;
	long	ebp;
	long	esi;
	long	edi;
	long	es;		/* 16 high bits zero */
	long	cs;		/* 16 high bits zero */
	long	ss;		/* 16 high bits zero */
	long	ds;		/* 16 high bits zero */
	long	fs;		/* 16 high bits zero */
	long	gs;		/* 16 high bits zero */
	long	ldt;		/* 16 high bits zero */
	short	t;
	short	io;
};

void set_init_task() {
	struct tss_struct init_tss = {
	 0,0,0x10,0,0,0,0,0, \
	 0,0,0,0,0,0,0,0,0,0,0x17,0x17,0x17,0x17,0x17,0x17, \
	 0x28, 0, 0 
	};
	struct desc_struct init_ldt[4] = {
		{0, 0}, {0xFFFF, 0x80EA10}, {0xFFFF, 0x80E210}, {0xFFFF, 0x80E210}
	};
	add_to_gdt((struct desc_struct *)GDT + 4, 104, &init_tss, 0x89);
	add_to_gdt((struct desc_struct *)GDT + 5, 32, &init_ldt[0], 0x82);
}
