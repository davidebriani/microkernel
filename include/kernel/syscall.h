#ifndef KERNEL_SYSCALL_H
#define KERNEL_SYSCALL_H

#include <kernel/stdint.h>

#define SYSCALL_SLOTS 256

/* Syscall numbers */
#define SYSCALL_PUTS 0x00
#define SYSCALL_PUTC 0x01
#define SYSCALL_HALT 0x02
#define SYSCALL_REBOOT 0x03
#define SYSCALL_LOAD 0x04

void (*syscall_get_function(uint32_t int_no))(void);
void syscall_setup(void);

#endif
