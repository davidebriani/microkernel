#ifndef KERNEL_SYSCALL_H
#define KERNEL_SYSCALL_H

#include <kernel/stdint.h>

uint32_t syscall_halt(void);
uint32_t syscall_reboot(void);
uint32_t syscall_putc(const int8_t c);
uint32_t syscall_puts(const int8_t *string);
uint32_t syscall_load(const int8_t *path);

#endif
