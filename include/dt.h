#ifndef DT_H
#define DT_H

#include "stdint.h"

/* Initialisation function is publicly accessible. */
void init_dt(void);

/* Update the TSS entry with the address of the correct kernel stack */
void task_set_stack(uint32_t stack);

/* This structure contains the value of one GDT entry.
*  We use the attribute 'packed' to tell GCC not to change
*  any of the alignment in the structure. */
struct gdt_entry_struct {
    uint16_t limit_low;		/* The lower 16 bits of the limit. */
    uint16_t base_low;		/* The lower 16 bits of the base. */
    uint8_t  base_middle;	/* The next 8 bits of the base. */
    uint8_t  access;		/* Access flags, determine what ring this segment can be used in. */
    uint8_t  granularity;
    uint8_t  base_high;		/* The last 8 bits of the base. */
} __attribute__((packed));

typedef struct gdt_entry_struct gdt_entry_t;

/* This struct describes a GDT pointer. It points to the start of
*  our array of GDT entries, and is in the format required by the
*  lgdt instruction. */
struct gdt_ptr_struct {
    uint16_t limit;		/* The upper 16 bits of all selector limits. */
    uint32_t base;		/* The address of the first gdt_entry_t struct. */
} __attribute__((packed));

typedef struct gdt_ptr_struct gdt_ptr_t;

/* A struct describing an interrupt gate. */
struct idt_entry_struct {
    uint16_t base_lo;		/* The lower 16 bits of the address to jump to when this interrupt fires. */
    uint16_t sel;		/* Kernel segment selector. */
    uint8_t  always0;		/* This must always be zero. */
    uint8_t  flags;		/* More flags. See documentation. */
    uint16_t base_hi;		/* The upper 16 bits of the address to jump to. */
} __attribute__((packed));

typedef struct idt_entry_struct idt_entry_t;

/* A struct describing a pointer to an array of interrupt handlers.
*  This is in a format suitable for giving to 'lidt'. */
struct idt_ptr_struct {
    uint16_t limit;
    uint32_t base;		/* The address of the first element in our idt_entry_t array. */
} __attribute__((packed));

typedef struct idt_ptr_struct idt_ptr_t;

/* A struct describing a Task State Segment */
struct tss_entry_struct {
    uint32_t prev_tss;
    uint32_t esp0;
    uint32_t ss0;
    uint32_t esp1;
    uint32_t ss1;
    uint32_t esp2;
    uint32_t ss2;
    uint32_t cr3;
    uint32_t eip;
    uint32_t eflags;
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    uint32_t es;
    uint32_t cs;
    uint32_t ss;
    uint32_t ds;
    uint32_t fs;
    uint32_t gs;
    uint32_t ldt;
    uint16_t trap;
    uint16_t iomap_base;
} __attribute__((packed));

typedef struct tss_entry_struct tss_entry_t;

/* These extern prev_tssdirectives let us access the addresses of our ASM ISR handlers. */
extern void isr0  (void);
extern void isr1  (void);
extern void isr2  (void);
extern void isr3  (void);
extern void isr4  (void);
extern void isr5  (void);
extern void isr6  (void);
extern void isr7  (void);
extern void isr8  (void);
extern void isr9  (void);
extern void isr10 (void);
extern void isr11 (void);
extern void isr12 (void);
extern void isr13 (void);
extern void isr14 (void);
extern void isr15 (void);
extern void isr16 (void);
extern void isr17 (void);
extern void isr18 (void);
extern void isr19 (void);
extern void isr20 (void);
extern void isr21 (void);
extern void isr22 (void);
extern void isr23 (void);
extern void isr24 (void);
extern void isr25 (void);
extern void isr26 (void);
extern void isr27 (void);
extern void isr28 (void);
extern void isr29 (void);
extern void isr30 (void);
extern void isr31 (void);
extern void irq0  (void);
extern void irq1  (void);
extern void irq2  (void);
extern void irq3  (void);
extern void irq4  (void);
extern void irq5  (void);
extern void irq6  (void);
extern void irq7  (void);
extern void irq8  (void);
extern void irq9  (void);
extern void irq10 (void);
extern void irq11 (void);
extern void irq12 (void);
extern void irq13 (void);
extern void irq14 (void);
extern void irq15 (void);
extern void isr128(void);

#endif
