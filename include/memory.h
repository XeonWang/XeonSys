#ifndef MEMORY_H
#define MEMORY_H

#define memory_copy(ds, source, es, target, length) {\
    __asm__("movw $40, %%bx \n\t" \
            "lldt %%bx \n\t" \
            "movw %%ax, %%es \n\t" \
            "rep; movsb" \
    :: "a" (es),"S" (source), "D" (target), "c" (length) : "bx");\
}

#endif // MEMORY_H
