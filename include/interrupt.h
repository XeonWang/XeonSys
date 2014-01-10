void clock_interrupt();
void sys_call_interrupt();
void set_interrupt(int index, void (*addr)(), unsigned short seg_selector, unsigned short dpl);
