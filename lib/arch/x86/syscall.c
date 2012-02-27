#include "lib/arch/x86/syscall.h"

#define DEFN_SYSCALL0(fn, num) \
uint32_t fn() \
{ \
  uint32_t a; \
  __asm__ __volatile__("int $0x80" : "=a" (a) : "0" (num)); \
  return a; \
}

#define DEFN_SYSCALL1(fn, num, P1) \
uint32_t fn(P1 p1) \
{ \
  uint32_t a; \
  __asm__ __volatile__("int $0x80" : "=a" (a) : "0" (num), "b" ((uint32_t)p1)); \
  return a; \
}

#define DEFN_SYSCALL2(fn, num, P1, P2) \
uint32_t fn(P1 p1, P2 p2) \
{ \
  uint32_t a; \
  __asm__ __volatile__("int $0x80" : "=a" (a) : "0" (num), "b" ((uint32_t)p1), "c" ((uint32_t)p2)); \
  return a; \
}

#define DEFN_SYSCALL3(fn, num, P1, P2, P3) \
uint32_t fn(P1 p1, P2 p2, P3 p3) \
{ \
  uint32_t a; \
  __asm__ __volatile__("int $0x80" : "=a" (a) : "0" (num), "b" ((uint32_t)p1), "c" ((uint32_t)p2), "d"((uint32_t)p3)); \
  return a; \
}

#define DEFN_SYSCALL4(fn, num, P1, P2, P3, P4) \
uint32_t fn(P1 p1, P2 p2, P3 p3, P4 p4) \
{ \
  uint32_t a; \
  __asm__ __volatile__("int $0x80" : "=a" (a) : "0" (num), "b" ((uint32_t)p1), "c" ((uint32_t)p2), "d" ((uint32_t)p3), "S" ((uint32_t)p4)); \
  return a; \
}

#define DEFN_SYSCALL5(fn, num) \
uint32_t fn(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) \
{ \
  uint32_t a; \
  __asm__ __volatile__("int $0x80" : "=a" (a) : "0" (num), "b" ((uint32_t)p1), "c" ((uint32_t)p2), "d" ((uint32_t)p3), "S" ((uint32_t)p4), "D" ((uint32_t)p5)); \
  return a; \
}

DEFN_SYSCALL1(syscall_puts, 0, const int8_t*)
DEFN_SYSCALL1(syscall_putc, 1, const int8_t)
