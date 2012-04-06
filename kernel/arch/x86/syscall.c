#include <kernel/stdint.h>
#include <kernel/arch/x86/isr.h>
#include <kernel/arch/x86/syscall.h>
#include <kernel/syscall.h>

static void syscall_handler(registers_t *regs);

void syscalls_init(void) {
    /* Register and connect functions to their syscall number */
    syscall_setup();

    /* Register our syscall handler. */
    irq_register_handler(IRQ_SYSCALL, &syscall_handler);
}

void syscall_handler(registers_t *regs) {
    int32_t ret;
    void *location;

    /* Get the required syscall callback */
    location = syscall_get_function(regs->eax);
    if (!location)
	return;

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
