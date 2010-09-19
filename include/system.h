#ifndef __SYSTEM_H
#define __SYSTEM_H

#include "stdint.h"

/* Install our Global Descriptor Table */
void init_dt();

/* This defines what the stack looks like after an ISR was running */
struct regs {
    //unsigned int gs, fs, es, ds;			/* pushed the segs last */
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;	/* pushed by 'pusha' */
    uint32_t int_no, err_code;				/* our 'push byte #' and ecodes do this */
    uint32_t eip, cs, eflags, useresp, ss;		/* pushed by the processor automatically */
};

typedef void (*isr_t)(struct regs *);

/* Installs a custom IRQ handler for the given IRQ  */
void irq_install_handler(uint32_t irq, isr_t handler);
/* Clears the handler for a given IRQ */
void irq_uninstall_handler(uint32_t irq);

/* A few defines to make life a little easier */
#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

/* Install the timer handler && setup the system clock */
void init_timer();
/* Set the timer phase in hz */
void timer_phase(int32_t hz);
/* Loop until the given time has been reached */
void timer_wait(int32_t ticks);

/* Install a custom handler in the IDT for the keyboard */
void init_keyboard();

#endif
