#include <kernel/stdint.h>
#include <kernel/arch/x86/isr.h>
#include <kernel/arch/x86/syscall.h>
#include <lib/arch/x86/syscall.h>
#include <kernel/syscall.h>

static void syscall_handler(registers_t *regs);

static void *syscalls[SYSCALL_SLOTS] = { 0 };

void syscalls_init(void)
{
    /* Register and connect functions to their syscall number */
    syscalls[SYSCALL_PUTS] = &syscall_puts;
    syscalls[SYSCALL_PUTC] = &syscall_putc;
    syscalls[SYSCALL_HALT] = &syscall_halt;
    syscalls[SYSCALL_REBOOT] = &syscall_reboot;
    syscalls[SYSCALL_LOAD] = &syscall_load;

    /* Register our syscall handler. */
    irq_register_handler(0x80, &syscall_handler);
}

void syscall_handler(registers_t *regs)
{
    int32_t ret;
    void *location;

    /* Firstly, check if the requested syscall number is valid.
    *  The syscall number is found in EAX. */
    if (regs->eax >= SYSCALL_SLOTS)
        return;

    /* Do we have a registered function for the specified syscall? */
    if (!syscalls[regs->eax])
	return;

    /* Get the required syscall location */
    location = syscalls[regs->eax];

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
    " : "=a" (ret) : "r" (regs->edi), "r" (regs->esi), "r" (regs->edx), "r" (regs->ecx), "r" (regs->ebx), "r" (location));
    regs->eax = ret;
}
