#include "textmode.h"
#include "dt.h"
#include "paging.h"
#include "timer.h"
#include "kb.h"
#include "heap.h"

int main(struct multiboot *mboot_ptr) {
    /* Four pointers for testing purpouses */
    void *a, *b, *c, *d;

    /* Setup the screen (by clearing it) */
    init_video();
    set_textcolor(LIGHTGREEN, BLACK);
    puts("# Video................ OK\n");

    /* Setup all the ISRs and segmentation */
    init_dt();
    puts("# GDT.................. OK\n");
    puts("# IDT.................. OK\n");

    /* Setup paging */
    init_paging();
    puts("# Paging............... OK\n");

    /* Init the system timer */
    init_timer();
    puts("# Timer/Clock.......... OK\n");

    /* Init the keyboard */
    init_keyboard();
    puts("# Keyboard (US)........ OK\n");

    set_textcolor(LIGHTRED, BLACK);

    /* Check if the system really works */
    puts("\n# Testing interrupts...\n");
    puts("\n");
    asm volatile("int $0x0");
    asm volatile("int $0x3");
    asm volatile("sti");	/* Important! Re-enable interrupt requests */

    /* Check if something went wrong with paging and heap setup */
    puts("\n# Testing the heap.....\n");
    a = (void *) kmalloc(8);
    b = (void *) kmalloc(8);
    c = (void *) kmalloc(8);
    kprintf("a: 0x%x\n", a);
    kprintf("b: 0x%x\n", b);
    kprintf("c: 0x%x\n", c);
    kfree(c);
    kfree(b);
    d = (void*)kmalloc(12);
    kprintf("d: 0x%x\n", d);
    kfree(a);
    kfree(d);

    /* Wait 1 second... */
    timer_wait(1);

    /* Print out something and change text colors */
    set_textcolor(LIGHTGREEN, BLACK);
    puts("\n# Aaaalright. Let's rock!\n\n");
    set_textcolor(WHITE, BLACK);

    return 0;
}
