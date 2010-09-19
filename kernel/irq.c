#include "system.h"
#include "ports.h"
#include "string.h"

/* This array is actually an array of function pointers. We use
*  this to handle custom IRQ handlers for a given IRQ */
isr_t interrupt_handlers[256];

/* This installs a custom IRQ handler for the given IRQ */
void irq_install_handler(uint32_t irq, isr_t handler) {
    interrupt_handlers[irq] = handler;
}

/* This clears the handler for a given IRQ */
void irq_uninstall_handler(uint32_t irq) {
    interrupt_handlers[irq] = 0;
}

/* Each of the IRQ ISRs point to this function, rather than
*  the 'fault_handler' in 'isrs.c'. The IRQ Controllers need
*  to be told when you are done servicing them, so you need
*  to send them an "End of Interrupt" command (0x20). There
*  are two 8259 chips: The first exists at 0x20, the second
*  exists at 0xA0. If the second controller (an IRQ from 8 to
*  15) gets an interrupt, you need to acknowledge the
*  interrupt at BOTH controllers, otherwise, you only send
*  an EOI command to the first controller. If you don't send
*  an EOI, you won't raise any more IRQs */
void irq_handler(struct regs r) {
    /* If the IDT entry that was invoked was greater than 40
    *  (meaning IRQ8 - 15), then we need to send an EOI to
    *  the slave controller */
    if (r.int_no >= 40)
	/* Send reset signal to slave interrupt controller */
        outportb(0xA0, 0x20);

    /* In either case, we need to send an EOI to the master
    *  interrupt controller too */
    outportb(0x20, 0x20);

    /* Find out if we have a custom handler to run for this
    *  IRQ, and then finally, run it */
    if (interrupt_handlers[r.int_no] != 0) {
	isr_t handler = interrupt_handlers[r.int_no];
	handler(&r);
    }
}
