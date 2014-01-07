
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
