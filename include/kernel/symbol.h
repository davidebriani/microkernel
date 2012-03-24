#ifndef KERNEL_SYMBOL_H
#define KERNEL_SYMBOL_H

#include <kernel/stdint.h>

#define SYMBOL_ENTRY_SLOTS 64
#define SYMBOL_MAP_SIZE 1024

struct symbol_entry {
    int8_t name[32];
    void *paddress;
};

void *symbol_find(const int8_t *name);
void symbol_init(void);

#endif

