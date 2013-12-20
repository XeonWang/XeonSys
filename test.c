#include <stdlib.h>

#define gdt(index, limit, addr, type) \
	__asm__("movw %%ax, %2\n\t" \
		"movw %%bx, %3\n\t" \
		"rorl $16, %%ebx\n\t" \
		"movb %%al, %4\n\t" \
		"movb $"type", %5\n\t" \
		::"a" (limit), "b" (addr), \
		"m" (table[index]), "m" (*((char *)(&table[index])+2)), \
		"m" (*((char *)(&table[index])+4)), \
		"m" (*((char *)(&table[index])+5)) \
	);


int main(void) {
	struct gdt_struct {
		long a, b;
	};
	int a = 12;

	struct gdt_struct *table = malloc(sizeof(struct gdt_struct)*10);
	gdt(2, 0x1000, &a, 0x89);
	return 0;
}
