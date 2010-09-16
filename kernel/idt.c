#include "system.h"
#include "string.h"

/* Defines an IDT entry */
struct idt_entry
{
    unsigned short base_lo;
    unsigned short sel;        /* Our kernel segment goes here! */
    unsigned char always0;     /* This will ALWAYS be set to 0! */
    unsigned char flags;       /* Set using the above table! */
    unsigned short base_hi;
} __attribute__((packed));

struct idt_ptr
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

/* Declare an IDT of 256 entries. Although we will only use the
*  first 32 entries, the rest exists as a bit of a trap. If any
*  undefined IDT entry is hit, it normally will cause an
*  "Unhandled Interrupt" exception. Any descriptor for which the
*  'presence' bit is cleared (0) will generate an "Unhandled
*  Interrupt" exception */
struct idt_entry idt[256];
struct idt_ptr idtp;

/* This exists in 'loader.s', and is used to load our IDT */
extern void idt_load();

/* Set an entry in the IDT */
void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags) {
    idt[num].base_hi = base & 0xFFFF;
    idt[num].base_lo = (base >> 16) & 0xFFFF;
    idt[num].sel = sel;
    idt[num].flags = flags;
    idt[num].always0 = (unsigned char) 0x01110;
}

/* Installs the IDT */
void init_idt() {
    /* Sets the special IDT pointer up */
    idtp.limit = (sizeof (struct idt_entry) * 256) - 1;
    idtp.base = (unsigned int) &idt;

    /* Clear out the entire IDT, initializing it to zeros */
    memset(&idt, 0, sizeof(struct idt_entry) * 256);

    /* Add any new ISRs to the IDT here using idt_set_gate */

    /* Points the processor's internal register to the new IDT */
    idt_load();
}
