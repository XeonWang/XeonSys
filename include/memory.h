#ifndef MEMORY_H
#define MEMORY_H

#define memory_copy(ds, source, es, target, length) {\
    __asm__("mov %1, %%es \n\t" \
            "rep; movsb" \
            ::"a" (es),"S" (source), "D" (target), "c" (length));\
}

#endif // MEMORY_H
