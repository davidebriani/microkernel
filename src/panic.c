#include "panic.h"
#include "textmode.h"

void panic(const char *message, const char *file, uint32_t line)
{
    /* Disable interrupts */
    asm volatile("cli");

    kprintf("KERNEL PANIC (%s) at %s:%d\n", message, file, line);

    /* Halt by going into an infinite loop. */
    for(;;);
}

void panic_assert(const char *file, uint32_t line, const char *desc)
{
    /* Disable interrupts */
    asm volatile("cli");

    kprintf("ASSERTION FAILED (%s) at %s:%d\n", desc, file, line);

    /* Halt by going into an infinite loop. */
    for(;;);
}
