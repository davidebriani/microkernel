#ifndef __SYSTEM_H
#define __SYSTEM_H

/* GDT stuff */
/* Install our Global Descriptor Table */
void init_gdt();

/* IDT stuff */
/* Install our Interrupt Descriptor Table */
void init_idt();
/* Set an entry in the IDT */
void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);

/* This defines what the stack looks like after an ISR was running */
struct regs {
    unsigned int gs, fs, es, ds;			/* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;/* pushed by 'pusha' */
    unsigned int int_no, err_code;			/* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;		/* pushed by the processor automatically */
};

/* Install our Interrupt Service Routines */
void init_isrs();

/* Remap the interrupt controllers, and then we install
* the appropriate ISRs to the correct entries in the IDT.*/
void init_irq();
/* Installs a custom IRQ handler for the given IRQ  */
void irq_install_handler(int irq, void (*handler)(struct regs *r));
/* Clears the handler for a given IRQ */
void irq_uninstall_handler(int irq);

/* Install the timer handler && setup the system clock */
void init_timer();
/* Set the timer phase in hz */
void timer_phase(int hz);
/* Loop until the given time has been reached */
void timer_wait(int ticks);

/* Install a custom handler in the IDT for the keyboard */
void init_keyboard();

#endif
