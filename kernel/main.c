#include "system.h"
#include "textmode.h"
#include "string.h"
#include "ports.h"

extern void halt();	/* defined in helper.s */

void kmain(void* mbd, unsigned int magic) {
    mbd = 0;
    magic = 1;

    init_video();
    set_textcolor(GREEN, BLACK);
    puts("Video OK.\n");

    init_gdt();
    puts("Setting up the GTD........\n");
    init_idt();

    puts("Setting up all ISRs.......\n");
    init_isrs();

    puts("Setting up all IRQs.......\n");
    init_irq();

    puts("Setting up the timer......\n");
    init_timer();

    puts("Setting up the keyboard...\n");
    init_keyboard();

    //__asm__ __volatile__ ("sti");

    set_textcolor(RED, BLACK);
    puts("\nTEST: Input from (US) keyboard:\n");
    set_textcolor(WHITE, BLACK);
    while(1) {
	//timer_wait(1);
	putch(inportb(0x60));
    }

    /*
    puts("Initialising physical memory management...\n");
    pmm_init(mboot_info);
    kprintf("kernel: 0x%x - 0x%x\nkernel-stack: 0x%x\n", &phys_kernel_start, &phys_kernel_end, kernel_stack);
    puts("Initiating multitasking...\n");
    init_multitasking();
    init_task(task_a, "task_a", 1);
    init_task(task_b, "task_b", 1);*/

    __asm__ __volatile__ ("sti");
}
