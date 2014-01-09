#ifndef MEMORY_H
#define MEMORY_H

#define memory_copy(ds, source, es, target, length) {\
    __asm__("sldt %%dx \n\t" \
            "movw $40, %%bx \n\t" \
            "lldt %%bx \n\t" \
            "movw %%ax, %%es \n\t" \
            "rep; movsb \n\t" \
            "lldt %%dx \n\t" \
    :: "a" (es),"S" (source), "D" (target), "c" (length) : "bx","dx");\
}

#endif // MEMORY_H
