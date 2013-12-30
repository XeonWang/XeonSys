int system_call(int number)
{
	int result;
	__asm__("int $0x80":"=a" (result):"a" (number));
	return result;
}
