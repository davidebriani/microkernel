#include "textmode.h"
#include "dt.h"
#include "paging.h"
#include "timer.h"
#include "kb.h"

int main(struct multiboot *mboot_ptr) {
    /* Setup all the ISRs and segmentation */
    init_dt();

    /* Setup the screen (by clearing it) */
    init_video();

    /* Setup paging */
    init_paging();

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
    asm volatile("sti");	/* Important! Re-enable interrupt requests */
    puts("\n");

    /* Init the system timer */
    init_timer();
    puts("# Timer................ OK\n");

    /* Init the keyboard */
    init_keyboard();
    puts("# Keyboard (US)........ OK\n");

    /* Wait 1 second... */
    timer_wait(1);

    /* Print out something and change text colors */
    puts("# Aaaalright. Let's rock!\n\n");
    set_textcolor(WHITE, BLACK);

    /* Force a page fault by reading location 0xA0000000 */
    uint32_t *ptr = (uint32_t *) 0xA0000000;
    uint32_t do_page_fault = *ptr;

    return 0;
}
