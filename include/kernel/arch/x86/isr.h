#ifndef KERNEL_ARCH_x86_IRS_H
#define KERNEL_ARCH_x86_IRS_H

#include <kernel/stdint.h>

/* A few defines to make life a little easier */
#define IRQ_DE      0x00
#define IRQ_DB      0x01
#define IRQ_NI      0x02
#define IRQ_BP      0x03
#define IRQ_OF      0x04
#define IRQ_BR      0x05
#define IRQ_UD      0x06
#define IRQ_NM      0x07
#define IRQ_DF      0x08
#define IRQ_CO      0x09
#define IRQ_TS      0x0A
#define IRQ_NP      0x0B
#define IRQ_SS      0x0C
#define IRQ_GP      0x0D
#define IRQ_PF      0x0E
#define IRQ_MF      0x10
#define IRQ_AC      0x11
#define IRQ_MC      0x12
#define IRQ_XM      0x13
#define IRQ_PIT     0x20
#define IRQ_KBD     0x21
#define IRQ_CASCADE 0x22
#define IRQ_COM2    0x23
#define IRQ_COM1    0x24
#define IRQ_SOUND   0x25
#define IRQ_SDA     0x26
#define IRQ_PP      0x27
#define IRQ_RTC     0x28
#define IRQ_MOUSE   0x2C
#define IRQ_FPU     0x2D
#define IRQ_ATAP    0x2E
#define IRQ_ATAS    0x2F
#define IRQ_SYSCALL 0x80

typedef struct registers {
    uint32_t ds;					/* Data segment selector */
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;	/* Pushed by pusha. */
    uint32_t int_no, err_code;				/* Interrupt number and error code (if applicable) */
    uint32_t eip, cs, eflags, useresp, ss;		/* Pushed by the processor automatically. */
} registers_t;

/* Enables registration of callbacks for interrupts or IRQs.
*  For IRQs, to ease confusion, use the #defines above as the
*  first parameter. */
typedef void (*isr_t)(registers_t*);
void irq_register_handler(uint32_t n, isr_t handler);
void irq_remove_handler(uint32_t n);
void isr_handler(registers_t regs);

#endif
