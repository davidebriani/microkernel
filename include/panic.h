#ifndef PANIC_H
#define PANIC_H

#include "stdint.h"

#define PANIC(msg)	panic(msg, __FILE__, __LINE__)

void panic(const char *message, const char *file, uint32_t line);

#endif
