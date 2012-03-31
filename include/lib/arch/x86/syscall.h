#ifndef LIB_ARCH_X86_SYSCALL_H
#define LIB_ARCH_X86_SYSCALL_H

#include <lib/stdint.h>

#define DECL_SYSCALL0(fn) uint32_t fn(void);
#define DECL_SYSCALL1(fn,p1) uint32_t fn(p1);
#define DECL_SYSCALL2(fn,p1,p2) uint32_t fn(p1,p2);
#define DECL_SYSCALL3(fn,p1,p2,p3) uint32_t fn(p1,p2,p3);
#define DECL_SYSCALL4(fn,p1,p2,p3,p4) uint32_t fn(p1,p2,p3,p4);
#define DECL_SYSCALL5(fn,p1,p2,p3,p4,p5) uint32_t fn(p1,p2,p3,p4,p5);

#define SYSCALL_PUTS 0x00
#define SYSCALL_PUTC 0x01
#define SYSCALL_HALT 0x02
#define SYSCALL_REBOOT 0x03
#define SYSCALL_LOAD 0x04

DECL_SYSCALL1(call_puts, const int8_t *)
DECL_SYSCALL1(call_putc, const int8_t)
DECL_SYSCALL0(call_halt)
DECL_SYSCALL0(call_reboot)
DECL_SYSCALL1(call_load, const int8_t *)

#endif
