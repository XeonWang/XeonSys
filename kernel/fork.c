#include <sys_call.h>

int fork()
{
	system_call(SYS_CALL_FORK);	
}

int fork_impl()
{
	__asm__("movw 0xFFFF, %ax");
}
