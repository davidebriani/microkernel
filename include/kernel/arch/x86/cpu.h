#ifndef KERNEL_ARCH_X86_CPU_H
#define KERNEL_ARCH_X86_CPU_H

#include <kernel/stdint.h>

void cpu_disable_interrupts(void);
void cpu_enable_fpu(void);
void cpu_enable_interrupts(void);
void cpu_enable_tss(void);
void cpu_enter_usermode(uint32_t ip, uint32_t sp);
uint32_t cpu_get_cr0(void);
uint32_t cpu_get_cr2(void);
uint32_t cpu_get_cr3(void);
uint32_t cpu_get_cr4(void);
uint32_t cpu_get_eflags(void);
uint32_t cpu_get_stack(void);
void cpu_halt(void);
void cpu_idle(void);
void cpu_set_cr0(uint32_t value);
void cpu_set_cr2(uint32_t value);
void cpu_set_cr3(uint32_t value);
void cpu_set_cr4(uint32_t value);
void cpu_set_gdt(void *pointer);
void cpu_set_idt(void *pointer);
void cpu_set_eflags(uint32_t value);
void cpu_set_stack(void *address);

#endif

