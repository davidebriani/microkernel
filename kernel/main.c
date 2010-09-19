#include "system.h"
#include "multiboot.h"
#include "textmode.h"
#include "string.h"
#include "ports.h"

extern void halt();	/* defined in helper.s */
uint32_t initial_esp;

int32_t kmain(struct multiboot *mboot_ptr, uint32_t initial_stack) {
    initial_esp = initial_stack;

    init_video();
    set_textcolor(GREEN, BLACK);
    puts("Video OK.\n");

    puts("Setting up the GDT/IDT....\n");
    init_dt();

    puts("Trying a division by 0....\n");
    int32_t i = 6/0;
/*
    puts("Trying int exceptions.....\n");
    asm volatile ("int $0x3");
*/
    puts("Setting up the timer......\n");
    init_timer();
    timer_phase(50);

    puts("Setting up the keyboard...\n");
    init_keyboard();

    set_textcolor(RED, BLACK);
    puts("\nTEST: Input from (US) keyboard:\n");
    set_textcolor(WHITE, BLACK);

    while(1) {
	//timer_wait(1);
	putch(inportb(0x60));
    }

    return 0xDEADBABA;
}
