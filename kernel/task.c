#include <descriptor.h>
#include <register.h>
#include <task.h>
#include <interrupt.h>
#include <const.h>

extern struct global_desc gdt[];
extern struct interrupt_desc idt[];
struct desc_struct ldt[MAX_PROCESS*4];
struct pcb_struct pcb[MAX_PROCESS];

struct pcb_struct processes[MAX_PROCESS];
struct pcb_struct *runable_processes[MAX_PROCESS];
struct pcb_struct *blocked_processes[MAX_PROCESS];

int get_empty_process() {
    int i;
    for(i = FIRST_PROCESS_INDEX ; i < MAX_PROCESS ; i++) {
        if(processes[i].pid == 0) {
            return i;
        }
    }
    return -1;
}

unsigned short get_next_pid() {
    unsigned short max_pid = 0;
    int i;
    for(i = 0 ; i < MAX_PROCESS ; i++) {
        if(processes[i].pid > max_pid) {
            max_pid = processes[i].pid;
        }
    }
    return max_pid + 1;
}

void set_global_ldt() {
    ldt[0].a = 0;
    ldt[0].b = 0;
    ldt[1].a = 0x0000ffff;
    ldt[1].b = 0xC0FA00;
    ldt[2].a = 0x0000ffff;
    ldt[2].b = 0xC0F200;
    ldt[3].a = 0x0000ffff;
    ldt[3].b = 0xC0F200;
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
//    set_global_ldt();

    ldt[0].a = 0;
    ldt[0].b = 0;
    ldt[1].a = 0x0000009f;
    ldt[1].b = 0xC0FA00;
    ldt[2].a = 0x0000009f;
    ldt[2].b = 0xC0F200;
    ldt[3].a = 0x0000009f;
    ldt[3].b = 0xC0F200;
    add_to_gdt(&gdt[4], 104, &init_tss, 0x89);
    add_to_gdt(&gdt[5], 8*256, &ldt[0], 0x82); //global ldt segment limit is ffff
    add_to_gdt(&gdt[6], 32, &ldt[0], 0x82);
	_LTR((unsigned short)(4<<3));
    _LLDT(6<<3);
    set_interrupt(0x20, &clock_interrupt, 8, 0);
    set_interrupt(0x80, &sys_call_interrupt, 8, 3);
}

short get_next_seg_index() {
//    return FIRST_PROCESS_INDEX;
    int i;
    for(i = FIRST_PROCESS_INDEX ; i < MAX_PROCESS ; i++) {
        if(gdt[i].limit_0_7 == NULL && gdt[i].limit_8_15 == NULL && gdt[i].limit_16_19 == NULL) {
            return i;
        }
    }
    return -1;
}

unsigned short get_task_index() {
    unsigned short index;
    __asm__("sldt %%eax \n\t" \
            "shr $3, %%eax \n\t" \
            "":"=ax" (index));
    return index;
}

unsigned short get_pid() {
    return pcb[get_task_index()].pid;
}

void to_runable(struct pcb_struct *task) {
    int i;
    for(i = FIRST_PROCESS_INDEX ; i < MAX_PROCESS ; i++) {
        if(runable_processes[i] == NULL) {
            runable_processes[i] = task;
            break;
        }
    }
}

void schedule() {
    unsigned short task_index = get_task_index();
    if(pcb[task_index].time_remain > 0) {
        pcb[task_index].time_remain--;
    } else {
        pcb[task_index].time_remain = TASK_TIMES;
        record_current_task_pcb();

        int i;
        for(i = task_index ; i < MAX_PROCESS ; i++) {
            if(runable_processes[i] == NULL) {
                load_task_pcb(index);
                return;
            }
        }
        for(i = FIRST_PROCESS_INDEX ; i < task_index ; i++) {
            if(runable_processes[i] == NULL) {
                load_task_pcb(index);
                return;
            }
        }
    }
}
