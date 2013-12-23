truct tss_struct {
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
};

struct desc_struct {
	unsigned long a,b;
}; 

#define FIRST_TSS_GD 32

void main() {
	char *video_memory = (char*) 0xB8000;
	*video_memory = 'H';
	*(video_memory+2) = 'e';
	*(video_memory+4) = 'l';
	*(video_memory+6) = 'l';
	*(video_memory+8) = 'o';

	struct tss_struct first_tss = {
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		
	}

	*(* struct desc_struct)(GDT + FIRST_TSS_GD) = 
}
