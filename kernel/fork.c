#include <sys_call.h>
#include <descriptor.h>
#include <task.h>
#include <memory.h>

extern struct global_desc gdt[];
extern struct desc_struct ldt[];
extern struct pcb_struct processes[];
extern struct pcb_struct *runable_processes[];
extern struct pcb_struct *blocked_processes[];
extern struct pcb_struct pcb[];

long get_next_seg_index();

int fork()
{
    unsigned short task_index = get_task_index();
    __asm__("movl %0, %%eax \n\t" \
            "addl $8, %%eax \n\t" \
            "movw %%cs, (%%eax) \n\t" \
            "addl $2, %%eax \n\t" \
            "movw %%ds, (%%eax) \n\t" \
            "addl $2, %%eax \n\t" \
            "movw %%es, (%%eax) \n\t" \
            "addl $2, %%eax \n\t" \
            "movw %%fs, (%%eax) \n\t" \
            "addl $2, %%eax \n\t" \
            "movw %%gs, (%%eax) \n\t" \
            "addl $2, %%eax \n\t" \
            "movw %%ss, (%%eax) \n\t" \
            "addl $2, %%eax \n\t" \
            "movl %%eax, (%%eax) \n\t" \
            "addl $4, %%eax \n\t" \
            "movl %%ebx, (%%eax) \n\t" \
            "addl $4, %%eax \n\t" \
            "movl %%ecx, (%%eax) \n\t" \
            "addl $4, %%eax \n\t" \
            "movl %%edx, (%%eax) \n\t" \
            "addl $4, %%eax \n\t" \
            "movl %%esp, (%%eax) \n\t" \
            "addl $4, %%eax \n\t" \
            "movl %%ebp, (%%eax) \n\t" \
            "addl $4, %%eax \n\t" \
            "movl %%esi, (%%eax) \n\t" \
            "addl $4, %%eax \n\t" \
            "movl %%edi, (%%eax) \n\t" \
            "addl $4, %%eax \n\t" \
            "jmp get_eip2 \n\t" \
            "get_eip: movl (%%esp), %%ebx \n\t" \
            "ret \n\t" \
             "get_eip2: call get_eip \n\t" \
            "movl %%ebx, (%%eax) \n\t" \
            "addl $4, %%eax \n\t" \
            "pushf \n\t" \
            "pushl %%ebx \n\t" \
            "movl 4(%%esp), %%ebx \n\t" \
            "movl %%ebx, (%%eax) \n\t" \
            "popl %%ebx \n\t" \
            "popf \n\t" \
    ::"p" (&pcb[task_index]):"eax", "ebx");
    __asm__(""::"b" (task_index));
    system_call(SYS_CALL_FORK);
}

void copy_registers(int src_index, int target_index) {
    pcb[target_index].cs = pcb[src_index].cs;
    pcb[target_index].ds = pcb[src_index].ds;
    pcb[target_index].es = pcb[src_index].es;
    pcb[target_index].fs = pcb[src_index].fs;
    pcb[target_index].gs = pcb[src_index].gs;
    pcb[target_index].ss = pcb[src_index].ss;
    pcb[target_index].eax = pcb[src_index].eax;
    pcb[target_index].ebx = pcb[src_index].ebx;
    pcb[target_index].ecx = pcb[src_index].ecx;
    pcb[target_index].edx = pcb[src_index].edx;
    pcb[target_index].esp = pcb[src_index].esp;
    pcb[target_index].ebp = pcb[src_index].ebp;
    pcb[target_index].esi = pcb[src_index].esi;
    pcb[target_index].edi = pcb[src_index].edi;
    pcb[target_index].eip = pcb[src_index].eip;
    pcb[target_index].eflags = pcb[src_index].eflags;
}

int fork_impl()
{
    unsigned short task_index;
    __asm__("":"=b" (task_index));
    long index = get_next_seg_index();
    unsigned long base_addr = 0x100000 + TASK_SIZE*index;
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
    processes[empty_pcb].ppid = pcb[task_index].pid;
    processes[empty_pcb].time_remain = 5;
    processes[empty_pcb].ldt_selector = index << 3;
    copy_registers(task_index, empty_pcb);

    memory_copy(0, 0, (((empty_pcb*4) << 3) + 7), 0, TASK_SIZE);

    to_runable(&processes[empty_pcb]);
}
