#include <sys_call.h>

int fork()
{
	system_call(SYS_CALL_FORK);	
}
