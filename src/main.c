#include "textmode.h"
#include "dt.h"
#include "timer.h"
#include "kb.h"

int main(struct multiboot *mboot_ptr) {
    /* Initialise all the ISRs and segmentation */
    init_dt();

    /* Initialise the screen (by clearing it) */
    init_video();

    /* Write out sample strings */
    set_textcolor(LIGHTGREEN, BLACK);
    puts("# Video................ OK\n");
    puts("# GDT.................. OK\n");
    puts("# IDT.................. OK\n");

    /* Check if the system really works */
    puts("# Testing interrupts...\n");
    puts("\n");
    asm volatile("int $0x0");
    asm volatile("int $0x3");
    asm volatile("sti");	/* Important! Allows interrupt requests */
    puts("\n");

    /* Init the system timer */
    init_timer();
    puts("# Timer................ OK\n");

    /* Init the keyboard */
    init_keyboard();
    puts("# Keyboard (US)........ OK\n");

    /* Wait 1 second... */
    timer_wait(1);

    set_textcolor(WHITE, BLACK);
    puts("\nAaaalright! You can start typing now:\n");

    return 0;
}
