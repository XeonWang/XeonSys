#include <descriptor.h>
#include <register.h>
#include <task.h>
#include <interrupt.h>

extern struct global_desc gdt[];
extern struct interrupt_desc idt[];
struct desc_struct ldt[MAX_PROCESS*4];
struct pcb_struct pcb[MAX_PROCESS];

pcb_struct processes[MAX_PROCESS];
pcb_struct *runable_processes[MAX_PROCESS];
pcb_struct *blocked_processes[MAX_PROCESS];

int get_empty_process() {
    int i;
    for(i = 0 ; i < MAX_PROCESS ; i++) {
        if(processes[i] == NULL) {
            return i;
        }
    }
    return -1;
}

unsigned short get_next_pid() {
    unsigned short max_pid = 0;
    int i;
    for(i = 0 ; i < MAX_PROCESS ; i++) {
        if(processes[i] != NULL && processes[i].pid > max_pid) {
            max_pid = processes[i].pid;
        }
    }
    return max_pid + 1;
}

void set_interrupt(int index, void (*addr)(), unsigned short seg_selector, unsigned short dpl) {
	idt[index].addr_0_15 = (unsigned short)addr; 
	idt[index].seg_selector = seg_selector; 
    idt[index].type = 0x8E00+(dpl<<13);
	idt[index].addr_16_31 = (unsigned short)((unsigned)addr>>16); 
}

void set_init_task() {
	struct tss_struct init_tss = {
     0,0xb00,0x10,0,0,0,0,0, \
	 0,0,0,0,0,0,0,0,0,0,0x17,0x17,0x17,0x17,0x17,0x17, \
	 0x28, 0, 0 
	};
    /*
	struct desc_struct init_ldt[4] = {
		{0, 0}, 
        {0x0000009f, 0xC0FA00},  //limit=0xFFFF, base_addr=0x0, type=1010, P_DPL_S=1111, G_D=1000
        {0x0000009f, 0xC0F200},  //limit=0xFFFF, base_addr=0x0, type=0010, P_DPL_S=1111, G_B=1000
		{0x0000009f, 0xC0F200}  //limit=0xFFFF, base_addr=0x100000, type=0010, P_DPL_S=1111, G_B=1000 
	};
    */
    ldt[0].a = 0;
    ldt[0].b = 0;
    ldt[1].a = 0x0000009f;
    ldt[1].b = 0xC0FA00;
    ldt[2].a = 0x0000009f;
    ldt[2].b = 0xC0F200;
    ldt[3].a = 0x0000009f;
    ldt[3].b = 0xC0F200;
    add_to_gdt(&gdt[4], 104, &init_tss, 0x89);
    add_to_gdt(&gdt[5], 32, &ldt[0], 0x82);
	_LTR((unsigned short)(4<<3));
	_LLDT(5<<3);
    set_interrupt(0x20, &clock_interrupt, 8, 0);
    set_interrupt(0x80, &sys_call_interrupt, 8, 3);
}

long get_next_seg_index() {
    return 0x38;
}
