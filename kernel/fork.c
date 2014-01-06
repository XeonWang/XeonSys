#include <sys_call.h>
#include <descriptor.h>
#include <task.h>

extern struct global_desc gdt[];
extern struct desc_struct ldt[];
extern pcb_struct processes[];
extern pcb_struct *runable_processes[];
extern pcb_struct *blocked_processes[];

long get_next_seg_index();

int fork()
{
	system_call(SYS_CALL_FORK);	
}

int fork_impl()
{
    __asm__("movw 0xFFFF, %ax");
    long index = get_next_seg_index();
    unsigned long base_addr = 0x100000 + 0x9f*4*1024*index;
    unsigned long base_addr_low16 = base_addr << 16;
    unsigned long base_addr_high8 = base_addr | 0xFF000000;
    unsigned long base_addr_middle8 = (base_addr | 0x00FF0000) >> 16;
    /*
    struct desc_struct init_ldt[4] = {
        {0, 0},
        {0x0000009f+base_addr_low16, 0xC0FA00+base_addr_middle8+base_addr_high8},  //limit=0x9f*4k=640K, base_addr=0x100000, type=1010, P_DPL_S=1111, G_D=1000
        {0x0000009f+base_addr_low16, 0xC0F200+base_addr_middle8+base_addr_high8},  //limit=0x9f*4k=640K, base_addr=0x100000, type=0010, P_DPL_S=1111, G_B=1000
        {0x0000009f+base_addr_low16, 0xC0F200+base_addr_middle8+base_addr_high8}  //limit=0x9f*4k=640K, base_addr=0x100000, type=0010, P_DPL_S=1111, G_B=1000
    };
    */

    ldt[index*4+0].a = 0;
    ldt[index*4+0].b = 0;
    ldt[index*4+1].a = 0x0000009f+base_addr_low16;
    ldt[index*4+1].b = 0xC0FA00+base_addr_middle8+base_addr_high8;
    ldt[index*4+2].a = 0x0000009f+base_addr_low16;
    ldt[index*4+2].b = 0xC0F200+base_addr_middle8+base_addr_high8;
    ldt[index*4+3].a = 0x0000009f+base_addr_low16;
    ldt[index*4+3].b = 0xC0F200+base_addr_middle8+base_addr_high8;

    add_to_gdt(&gdt[index], 32, &ldt[index*4], 0x82);

    int empty_pcb = get_empty_process();
    if(empty_pcb == -1) return -1;
    processes[empty_pcb].pid = get_next_pid();
    processes[empty_pcb].ppid = 0;  //TODO set as appropriate value
    processes[empty_pcb].time_remain = 5;
    processes[empty_pcb].ldt_selector = index << 3;
}
