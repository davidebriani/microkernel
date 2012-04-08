#include <kernel/syscall.h>
#include <kernel/kernel.h>
#include <kernel/vfs.h>
#include <kernel/log.h>

static void (*syscalls[SYSCALL_SLOTS])(void) = { 0 };

/* Here goes all static uint32_t _syscall(*) definitions */

static uint32_t _halt(void) {
    kernel_halt();
    return 0;
}

static uint32_t _reboot(void) {
    kernel_reboot();
    return 0;
}

static uint32_t _putc(const int8_t c) {
    log_write("%c", c);
    return 0;
}

static uint32_t _puts(const int8_t *string) {
    log_write("%s", string);
    return 0;
}

static uint32_t _load(const int8_t *path) {
    void *physical;
    void (*init)(void) = 0;

    physical = vfs_get_physical(path);

    if (!physical)
        return 0;

/* TODO: ELF relocation

    elf_relocate(physical);
    init = (void (*)(void)) elf_get_symbol(physical, "init");

*/

    if (!init)
        return 0;

    init();

    return 1;
}

void (*syscall_get_function(uint32_t int_no))(void) {
    /* Check if the requested syscall number is valid */
    if (int_no >= SYSCALL_SLOTS)
	return 0;

    /* Do we have a registered callback for the specified syscall? */
    if (!syscalls[int_no])
	return 0;
    else
	return syscalls[int_no];
}

void syscall_setup(void) {
    /* Register and connect functions to their syscall number */
    syscalls[SYSCALL_PUTS] =	(void (*)(void)) &_puts;
    syscalls[SYSCALL_PUTC] =	(void (*)(void)) &_putc;
    syscalls[SYSCALL_HALT] =	(void (*)(void)) &_halt;
    syscalls[SYSCALL_REBOOT] =	(void (*)(void)) &_reboot;
    syscalls[SYSCALL_LOAD] =	(void (*)(void)) &_load;
}
