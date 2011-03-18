#include <kernel/panic.h>
#include <kernel/kernel.h>
#include <kernel/log.h>

static void panic_do(const int8_t *file, uint32_t line) {
    log_write("# FILE: %s:%d\n", file, line);
    log_write("# SYSTEM HALTED!\n");

    /* Disable interrupts */
    kernel_disable_interrupts();

    /* Halt by going into an infinite loop */
    for(;;);

    /* Just in case... */
    kernel_halt();
}

void panic(const int8_t *message, const int8_t *file, uint32_t line) {
    log_write("\n# KERNEL PANIC: %s\n", message);
    panic_do(file, line);
}

void panic_assert(const int8_t *assertion, const int8_t *file, uint32_t line) {
    log_write("\n# ASSERTION FAILED: %s\n", assertion);
    panic_do(file, line);
}
