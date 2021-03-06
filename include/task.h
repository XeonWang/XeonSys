#define TASK_SIZE 0x9f*4*1024

#define record_current_task_to_pcb(task_index) {\
    __asm__("movl %0, %%eax \n\t" \
            "addl $8, %%eax \n\t" \
            "movw 0x2C(%%esp), %%bx \n\t" \
            "movw %%bx, (%%eax) \n\t" \
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
            "movl 0x28(%%esp), %%ebx \n\t" \
            "movl %%ebx, (%%eax) \n\t" \
            "addl $4, %%eax \n\t" \
            "pushf \n\t" \
            "pushl %%ebx \n\t" \
            "movl 4(%%esp), %%ebx \n\t" \
            "movl %%ebx, (%%eax) \n\t" \
            "popl %%ebx \n\t" \
            "popf \n\t" \
    ::"p" (&pcb[task_index]):"eax", "ebx"); \
}

#define load_task_pcb(task_index) {\
    __asm__("movl %0, %%eax \n\t" \
            "addl $8, %%eax \n\t" \
            "addl $2, %%eax \n\t" \
            "movw (%%eax), %%ds \n\t" \
            "addl $2, %%eax \n\t" \
            "movw (%%eax), %%es \n\t" \
            "addl $2, %%eax \n\t" \
            "movw (%%eax), %%fs \n\t" \
            "addl $2, %%eax \n\t" \
            "movw (%%eax), %%gs \n\t" \
            "addl $2, %%eax \n\t" \
            "movw (%%eax), %%ss \n\t" \
            "addl $2, %%eax \n\t" \
            "movl (%%eax), %%eax \n\t" \
            "addl $4, %%eax \n\t" \
            "addl $4, %%eax \n\t" \
            "movl (%%eax), %%ecx \n\t" \
            "addl $4, %%eax \n\t" \
            "movl (%%eax), %%edx \n\t" \
            "addl $4, %%eax \n\t" \
            "movl (%%eax), %%esp \n\t" \
            "addl $4, %%eax \n\t" \
            "movl (%%eax), %%ebp \n\t" \
            "addl $4, %%eax \n\t" \
            "movl (%%eax), %%esi \n\t" \
            "addl $4, %%eax \n\t" \
            "movl (%%eax), %%edi \n\t" \
            "addl $4, %%eax \n\t" \
            "addl $4, %%eax \n\t" \
            "pushl (%%eax) \n\t" \
            "popf \n\t" \
            "lldt  %%bx \n\t" \
            "pushw -48(%%eax) \n\t" \
            "pushl -4(%%eax) \n\t" \
            "movb $0x20, %%al \n\t" \
            "outb %%al, $0x20 \n\t" \
            "lret \n\t" \
    ::"p" (&pcb[task_index]), "b" (task_index<<3):"eax"); \
}

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

#pragma pack (2)
struct pcb_struct {
    unsigned short pid;
    unsigned short ppid;
    unsigned short ldt_selector;
    unsigned short time_remain;
    unsigned short cs;
    unsigned short ds;
    unsigned short es;
    unsigned short fs;
    unsigned short gs;
    unsigned short ss;
    int eax;
    int ebx;
    int ecx;
    int edx;
    int esp;
    int ebp;
    int esi;
    int edi;
    int eip;
    int eflags;
};
#pragma pack ()

void set_init_task();
int get_empty_process();
unsigned short get_next_pid();
unsigned short get_pid();
unsigned short get_task_index();

