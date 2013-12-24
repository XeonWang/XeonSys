#include <descriptor.h>
#include <register.h>
#include <task.h>

extern gdt;


void set_init_task() {
	struct tss_struct init_tss = {
	 0,0,0x10,0,0,0,0,0, \
	 0,0,0,0,0,0,0,0,0,0,0x17,0x17,0x17,0x17,0x17,0x17, \
	 0x28, 0, 0 
	};
	struct desc_struct {
		long a, b;
	};
	struct desc_struct init_ldt[4] = {
		{0, 0}, {0xFFFF, 0x80EA10}, {0xFFFF, 0x80E210}, {0xFFFF, 0x80E210}
	};
	struct global_desc *first_tss_seg = (struct global_desc *)gdt + 4;
	struct global_desc *first_ldt_seg = (struct global_desc *)gdt + 5;
	add_to_gdt(first_tss_seg, 104, &init_tss, 0x89);
	add_to_gdt(first_ldt_seg, 32, &init_ldt[0], 0x82);
}
