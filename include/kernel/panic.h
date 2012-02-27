#ifndef PANIC_H
#define PANIC_H

#include "kernel/stdint.h"

#define PANIC(msg)	panic(msg, __FILE__, __LINE__)
#define ASSERT(a)	((a) ? (void)0 : panic_assert(#a, __FILE__, __LINE__))

void panic(const int8_t *message, const int8_t *file, uint32_t line);
void panic_assert(const int8_t *assertion, const int8_t *file, uint32_t line);

#endif
