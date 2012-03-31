#include <kernel/syscall.h>
#include <kernel/kernel.h>
#include <kernel/vfs.h>
#include <kernel/video/vga.h>

static void *syscalls[SYSCALL_SLOTS] = { 0 };

/* TODO: Declare syscall_* functions as static */

uint32_t syscall_halt(void) {
    kernel_halt();
    return 0;
}

uint32_t syscall_reboot(void) {
    kernel_reboot();
    return 0;
}

uint32_t syscall_putc(const int8_t c) {
    putc(c);
    return 0;
}

uint32_t syscall_puts(const int8_t *string) {
    puts(string);
    return 0;
}

uint32_t syscall_load(const int8_t *path) {
    void *physical;
    void (*init)(void) = 0;

    physical = vfs_get_physical(path);

    if (!physical)
        return 0;

/* TODO: ELF relocation

    elf_relocate(physical);
    init = elf_get_symbol(physical, "init");

*/

    if (!init)
        return 0;

    init();

    return 1;
}

void *syscall_get_function(int32_t fn) {
    /* Check if the requested syscall number is valid */
    if (fn >= SYSCALL_SLOTS)
	return 0;

    /* Do we have a registered callback for the specified syscall? */
    if (!syscalls[fn])
	return 0;
    else
	return syscalls[fn];
}

void syscall_setup(void) {
    /* Register and connect functions to their syscall number */
    syscalls[SYSCALL_PUTS] = &syscall_puts;
    syscalls[SYSCALL_PUTC] = &syscall_putc;
    syscalls[SYSCALL_HALT] = &syscall_halt;
    syscalls[SYSCALL_REBOOT] = &syscall_reboot;
    syscalls[SYSCALL_LOAD] = &syscall_load;
}
