#include "isr.h"
#include "textmode.h"
#include "ports.h"

/* This array is actually an array of function pointers. We use
*  this to handle custom IRQ handlers for a given IRQ */
isr_t interrupt_handlers[256];

/* This is a simple string array. It contains the message that
*  corresponds to each and every exception. We get the correct
*  message by accessing like: exception_message[interrupt_numr] */
const int8_t *exception_messages[] = {
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

/* This installs a custom IRQ handler for the given IRQ */
void register_interrupt_handler(uint32_t n, isr_t handler) {
    interrupt_handlers[n] = handler;
}

/* This clears the handler for the given IRQ */
void remove_interrupt_handler(uint32_t n) {
    interrupt_handlers[n] = 0;
}

/* This gets called from our ASM interrupt handler stub. */
void isr_handler(registers_t regs) {

    /* This line is important. When the processor extends the 8-bit interrupt number
    *  to a 32bit value, it sign-extends, not zero extends. So if the most significant
    *  bit (0x80) is set, regs.int_no will be very large (about 0xffffff80). */
    uint8_t int_no = regs.int_no & 0xFF;

    if (interrupt_handlers[int_no] != 0) {
        isr_t handler = interrupt_handlers[int_no];
        handler(&regs);
    }
    else {
        /* Is this a fault whose number is from 0 to 31? */
        if (int_no < 32) {
            /* Display the description for the Exception that occurred.
            *  In this tutorial, we will simply halt the system using an
            *  infinite loop */
	    puts("Alert! Ignoring interrupt: ");
            puts(exception_messages[int_no]);
            puts(" Exception.\n");
        }
        else {
            kprintf("Unhandled interrupt: %x! System Halted!\n", int_no);
            for (;;);
        }
    }
}

/* This gets called from our ASM interrupt handler stub.
*  The IRQ Controllers need to be told when you are done
*  servicing them, so you need to send them an "End of
*  Interrupt" command (0x20). There are two 8259 chips:
*  The first exists at 0x20, the second exists at 0xA0.
*  If the second controller (an IRQ from 8 to 15) gets
*  an interrupt, you need to acknowledge the interrupt
*  at BOTH controllers, otherwise, you only send an EOI
*  command to the first controller. If you don't send
*  an EOI, you won't raise any more IRQs */
void irq_handler(registers_t regs) {
    /* Send an EOI (end of interrupt) signal to the PICs.*/

    /* If the IDT entry that was invoked was greater than 40
    *  (meaning IRQ8 - 15), then we need to send an EOI to
    *  the slave controller */
    if (regs.int_no >= 40) {
        /* Send reset signal to slave. */
        outb(0xA0, 0x20);
    }
    /* Send reset signal to master. (As well as slave, if necessary). */
    outb(0x20, 0x20);

    /* Find out if we have a custom handler to run for this
    *  IRQ, and then finally, run it */
    if (interrupt_handlers[regs.int_no] != 0) {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(&regs);
    }

}
