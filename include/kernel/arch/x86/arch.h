#ifndef KERNEL_ARCH_X86_ARCH_H
#define KERNEL_ARCH_X86_ARCH_H

#include <kernel/kernel.h>
#include <kernel/mboot.h>

typedef kernel_arch arch_x86;

void arch_init(uint32_t magic, multiboot_header_t *header, void *stack);

#endif

