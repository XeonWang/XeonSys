#include <descriptor.h>
#include <register.h>
#include <task.h>
#include <interrupt.h>

extern struct global_desc gdt[];
extern struct interrupt_desc idt[];

void set_interrupt(int index, void (*addr)(), unsigned short seg_selector) {
	idt[index].addr_0_15 = (unsigned short)addr; 
	idt[index].seg_selector = seg_selector; 
	idt[index].type = 0x8E00; 
	idt[index].addr_16_31 = (unsigned short)((unsigned)addr>>16); 
}

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
		{0, 0}, 
		{0xFFFF, 0x80EA10},  //limit=0xFFFF, base_addr=0x100000, type=1010, P_DPL_S=1111, G_D=1000
		{0xFFFF, 0x80E210},  //limit=0xFFFF, base_addr=0x100000, type=0010, P_DPL_S=1111, G_B=1000 
		{0xFFFF, 0x80E210}
	};
	add_to_gdt(&gdt[4], 104, &init_tss, 0x89);
	add_to_gdt(&gdt[5], 32, &init_ldt[0], 0x82);
	_LTR((unsigned short)(4<<3));
	_LLDT(5<<3);
	set_interrupt(0x20, &clock_interrupt, 8);
}
