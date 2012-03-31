#ifndef KERNEL_SYSCALL_H
#define KERNEL_SYSCALL_H

#include <kernel/stdint.h>

#define SYSCALL_SLOTS 256

#define SYSCALL_PUTS 0x00
#define SYSCALL_PUTC 0x01
#define SYSCALL_HALT 0x02
#define SYSCALL_REBOOT 0x03
#define SYSCALL_LOAD 0x04

uint32_t syscall_halt(void);
uint32_t syscall_reboot(void);
uint32_t syscall_putc(const int8_t c);
uint32_t syscall_puts(const int8_t *string);
uint32_t syscall_load(const int8_t *path);

void *syscall_get_function(int32_t fn);
void syscall_setup(void);

#endif
