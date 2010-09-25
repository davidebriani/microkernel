#include "panic.h"
#include "textmode.h"
#include "system.h"

static void panic_do(const int8_t *file, uint32_t line)
{
    kprintf("FILE:\t%s:%d\n", file, line);
    kprintf("SYSTEM HALTED!\n");

    /* Disable interrupts */
    cli();

    /* Halt by going into an infinite loop */
    for(;;);

    /* Just in case... */
    halt();
}

void panic(const int8_t *message, const int8_t *file, uint32_t line)
{
    kprintf("\n\nKERNEL PANIC: %s\n", message);
    panic_do(file, line);
}

void panic_assert(const int8_t *assertion, const int8_t *file, uint32_t line)
{
    kprintf("\n\nASSERTION FAILED: %s\n", assertion);
    panic_do(file, line);
}
