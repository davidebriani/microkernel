#ifndef SYSCALL_H
#define SYSCALL_H

#include "stdint.h"

void init_syscalls(void);

#define DECL_SYSCALL0(fn) int32_t syscall_##fn();
#define DECL_SYSCALL1(fn,p1) int32_t syscall_##fn(p1);
#define DECL_SYSCALL2(fn,p1,p2) int32_t syscall_##fn(p1,p2);
#define DECL_SYSCALL3(fn,p1,p2,p3) int32_t syscall_##fn(p1,p2,p3);
#define DECL_SYSCALL4(fn,p1,p2,p3,p4) int32_t syscall_##fn(p1,p2,p3,p4);
#define DECL_SYSCALL5(fn,p1,p2,p3,p4,p5) int32_t syscall_##fn(p1,p2,p3,p4,p5);

#define DEFN_SYSCALL0(fn, num) \
int32_t syscall_##fn() \
{ \
  int32_t a; \
  __asm__ __volatile__("int $0x80" : "=a" (a) : "0" (num)); \
  return a; \
}

#define DEFN_SYSCALL1(fn, num, P1) \
int32_t syscall_##fn(P1 p1) \
{ \
  int32_t a; \
  __asm__ __volatile__("int $0x80" : "=a" (a) : "0" (num), "b" ((int32_t)p1)); \
  return a; \
}

#define DEFN_SYSCALL2(fn, num, P1, P2) \
int32_t syscall_##fn(P1 p1, P2 p2) \
{ \
  int32_t a; \
  __asm__ __volatile__("int $0x80" : "=a" (a) : "0" (num), "b" ((int32_t)p1), "c" ((int32_t)p2)); \
  return a; \
}

#define DEFN_SYSCALL3(fn, num, P1, P2, P3) \
int32_t syscall_##fn(P1 p1, P2 p2, P3 p3) \
{ \
  int32_t a; \
  __asm__ __volatile__("int $0x80" : "=a" (a) : "0" (num), "b" ((int32_t)p1), "c" ((int32_t)p2), "d"((int32_t)p3)); \
  return a; \
}

#define DEFN_SYSCALL4(fn, num, P1, P2, P3, P4) \
int32_t syscall_##fn(P1 p1, P2 p2, P3 p3, P4 p4) \
{ \
  int32_t a; \
  __asm__ __volatile__("int $0x80" : "=a" (a) : "0" (num), "b" ((int32_t)p1), "c" ((int32_t)p2), "d" ((int32_t)p3), "S" ((int32_t)p4)); \
  return a; \
}

#define DEFN_SYSCALL5(fn, num) \
int32_t syscall_##fn(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) \
{ \
  int32_t a; \
  __asm__ __volatile__("int $0x80" : "=a" (a) : "0" (num), "b" ((int32_t)p1), "c" ((int32_t)p2), "d" ((int32_t)p3), "S" ((int32_t)p4), "D" ((int32_t)p5)); \
  return a; \
}

DECL_SYSCALL1(puts, const int8_t *)
DECL_SYSCALL1(putc, const int8_t)

#endif
