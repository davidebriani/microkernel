#include "panic.h"
#include "string.h"

void panic(const char *message, const char *file, uint32_t line)
{
    asm volatile("cli"); /* Disable interrupts. */

    kprintf("KERNEL PANIC (%s) at %s:%d\n", message, file, line);

    /* Halt by going into an infinite loop. */
    for(;;);
}
