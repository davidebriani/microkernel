#include "system.h"
#include "string.h"

extern isr_t interrupt_handlers[];

/* This is a simple string array. It contains the message that
*  corresponds to each and every exception. We get the correct
*  message by accessing like: exception_message[interrupt_numr] */
int8_t *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",
    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

/* All of our Exception handling Interrupt Service Routines will
*  point to this function. This will tell us what exception has
*  happened! Right now, we simply halt the system by hitting an
*  endless loop. All ISRs disable interrupts while they are being
*  serviced as a 'locking' mechanism to prevent an IRQ from
*  happening and messing up kernel data structures */
void isr_handler(struct regs r) {
    /* This line is important. When the processor extends the 8-bit interrupt number
    *  to a 32bit value, it sign-extends, not zero extends. So if the most significant
    *  bit (0x80) is set, regs.int_no will be very large (about 0xffffff80). */
    uint8_t int_no = r.int_no & 0xFF;

    if (interrupt_handlers[int_no] != 0) {
	isr_t handler = interrupt_handlers[int_no];
	handler(&r);
    }
    else {
	/* Is this a fault whose number is from 0 to 31? */
	if (int_no < 32) {
	    /* Display the description for the Exception that occurred.
	    *  In this tutorial, we will simply halt the system using an
	    *  infinite loop */
	    puts(exception_messages[int_no]);
	    puts(" Exception. System Halted!\n");
	    for (;;);
	}
	else {
	    kprintf("Unhandled interrupt: %x. System Halted!\n", int_no);
	    for (;;);
	}
    }
}
