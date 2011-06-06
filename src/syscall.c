#include "stdint.h"
#include "syscall.h"
#include "isr.h"
#include "dt.h"
#include "textmode.h"

static void syscall_handler(registers_t regs);

DEFN_SYSCALL1(puts, 0, const int8_t*)

static void *syscalls[1] =
{
    &puts,
};

uint32_t num_syscalls = 1;

void init_syscalls()
{
    /* Register our syscall handler. */
    register_interrupt_handler(0x80, &syscall_handler);
}

void syscall_handler(registers_t regs)
{
    int32_t ret;
    void *location;

    /* Firstly, check if the requested syscall number is valid.
    *  The syscall number is found in EAX. */
    if (regs.eax >= num_syscalls)
        return;

    /* Get the required syscall location */
    location = syscalls[regs.eax];

    /* We don't know how many parameters the function wants, so we just push
    *  them all onto the stack in the correct order. The function will use all
    *  the parameters it wants, and we can pop them all back off afterwards */
    __asm__ __volatile__(" \
      push %1; \
      push %2; \
      push %3; \
      push %4; \
      push %5; \
      call *%6; \
      pop %%ebx; \
      pop %%ebx; \
      pop %%ebx; \
      pop %%ebx; \
      pop %%ebx; \
    " : "=a" (ret) : "r" (regs.edi), "r" (regs.esi), "r" (regs.edx), "r" (regs.ecx), "r" (regs.ebx), "r" (location));
    regs.eax = ret;
}
