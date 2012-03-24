#include <kernel/syscall.h>
#include <kernel/kernel.h>
#include <kernel/vfs.h>
#include <kernel/video/vga.h>

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
    void (*init)(void);

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


/* TODO: something like this

uint32_t syscall_read(uint32_t stack) {
    const int8_t *path;
    uint32_t offset, count;
    int8_t *buffer;

    path   = *(const int8_t **)(stack + 4);
    offset = *(uint32_t *)(stack + 8);
    count  = *(uint32_t *)(stack + 12);
    buffer = *(int8_t **)(stack + 16);

    return vfs_read(path, offset, count, buffer);
}

*/
