#ifndef KERNEL_ARCH_x86_IO_H
#define KERNEL_ARCH_x86_IO_H

#include <kernel/stdint.h>

void	 io_outb(uint16_t port, uint8_t value);
void	 io_outw(uint16_t port, uint16_t value);
void	 io_outd(uint16_t port, uint32_t value);
uint8_t	 io_inb(uint16_t port);
uint16_t io_inw(uint16_t port);
uint32_t io_ind(uint16_t port);

#endif
