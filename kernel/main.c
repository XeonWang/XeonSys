#include <descriptor.h>
#include <register.h>
#include <fork.h>

struct global_desc gdt[256];
struct interrupt_desc idt[256];

void set_global_seg()
{
	gdt[0].limit_0_7 = 0x0;
	gdt[0].limit_8_15 = 0x0;
	gdt[0].base_0_7 = 0x0;
	gdt[0].base_8_15 = 0x0;
	gdt[0].base_16_23 = 0x0;
	gdt[0].type = 0x0; 
	gdt[0].limit_16_19 = 0x0; 
	gdt[0].base_24_31 = 0x0;

	gdt[1].limit_0_7 = 0xFF;
	gdt[1].limit_8_15 = 0xFF;
	gdt[1].base_0_7 = 0x0;
	gdt[1].base_8_15 = 0x0;
	gdt[1].base_16_23 = 0x0;
	gdt[1].type = 0x9A; //P=1, DPL=0, S=1, type=1010
	gdt[1].limit_16_19 = 0xCF; //G=1(4k), D=1(32bit)
	gdt[1].base_24_31 = 0x0;

	gdt[2].limit_0_7 = 0xFF;
	gdt[2].limit_8_15 = 0xFF;
	gdt[2].base_0_7 = 0x0;
	gdt[2].base_8_15 = 0x0;
	gdt[2].base_16_23 = 0x0;
	gdt[2].type = 0x92; //P=1, DPL=0, S=1, type=1010
	gdt[2].limit_16_19 = 0xCF; //G=1(4k), D=1(32bit)
	gdt[2].base_24_31 = 0x0;
}

void test_C()
{
	char *video_memory = (char*) 0xB8000;
	*video_memory = 'H';
	*(video_memory+2) = 'e';
	*(video_memory+4) = 'l';
	*(video_memory+6) = 'l';
	*(video_memory+8) = 'o';
}

void main() {
	test_C();
	
	set_global_seg();
	struct desc_desc gdt_desc = {256*8-1, (unsigned long)&gdt[0]};
	_LGDT(gdt_desc);
	struct desc_desc idt_desc = {256*8-1, (unsigned long)&idt[0]};
	_LIDT(idt_desc);
	
	set_init_task();
	_STI();
	change_to_user_mode();
	
	fork();
}
