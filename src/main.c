#include "kernel.h"

extern uint32_t placement_address;
uint32_t initial_esp;

static void kernel_init(struct multiboot *mboot_ptr, uint32_t initial_stack);
static void kernel_test(void);
static void kernel_halt(void);

int main(struct multiboot *mboot_ptr, uint32_t initial_stack)
{
    kernel_init(mboot_ptr, initial_stack);
    kernel_test();
    sti();

    init_shell();

    kernel_halt();

    return 0;
}

static void kernel_init(struct multiboot *mboot_ptr, uint32_t initial_stack) {
    initial_esp = initial_stack;

    /* Setup all the ISRs and segmentation */
    init_dt();

    /* Setup the screen (by clearing it) */
    init_textmode();
    textmode_color(LIGHTGREEN, BLACK);
    puts("# GDT.................. OK\n");
    puts("# IDT.................. OK\n");
    puts("# Screen (text mode)... OK\n");

    /* Init the system timer */
    init_timer();
    puts("# Timer/Clock.......... OK\n");

    /* Init the keyboard */
    init_keyboard();
    puts("# Keyboard (US)........ OK\n");

    /* Find the location of our initial ramdisk which should have
    *  been loaded as a module by the boot loader */
    ASSERT(mboot_ptr->mods_count > 0);
    uint32_t initrd_location = *((uint32_t *)mboot_ptr->mods_addr);
    uint32_t initrd_end = *(uint32_t *)(mboot_ptr->mods_addr + 4);
    /* Don't trample our module with placement accesses, please! */
    placement_address = initrd_end;

    /* Initialise the initial ramdisk, and set it as the filesystem root */
    fs_root = init_initrd(initrd_location);

    /* Setup paging */
    init_paging();
    puts("# Paging............... OK\n");

#if DEBUG
    /* Setup multitasking */
    init_tasking();
    puts("# Multitasking......... OK\n");
#endif

    /* Setup syscalls */
    init_syscalls();
    syscall_puts("# Syscalls............. OK\n");

#if DEBUG
    /* Switch to user mode */
    init_usermode();
    syscall_puts("# Usermode..............OK\n");
#endif

    textmode_color(WHITE, BLACK);
}

static void kernel_test(void) {
    textmode_color(LIGHTRED, BLACK);

    /* Check if the system really works */
    puts("# Testing interrupts...\n");
    __asm__ __volatile__("int $0x0");
    __asm__ __volatile__("int $0x3");
    /* Important! Re-enable interrupt requests */
    sti();

    /* Check if something went wrong with paging and heap setup */
    puts("# Testing the heap.....\n");
    void *a = (void *) kmalloc(8);
    void *b = (void *) kmalloc(8);
    void *c = (void *) kmalloc(8);
    kprintf("a: 0x%x\n", a);
    kprintf("b: 0x%x\n", b);
    kprintf("c: 0x%x\n", c);
    kfree(c);
    kfree(b);
    void *d = (void *) kmalloc(12);
    kprintf("d: 0x%x\n", d);
    kfree(a);
    kfree(d);

    /* Try to read files from the ramdisk image */
    puts("# Testing initrd.img...\n");
    /* list the contents of / */
    int32_t i = 0;
    struct dirent *node = 0;
    while ( (node = readdir(fs_root, i)) != 0)
    {
	fs_node_t *fsnode = finddir(fs_root, node->name);
	if ((fsnode->flags & 0x7) == FS_DIRECTORY)
	    kprintf("dr--r--r--\t%s\n", node->name);
	else
	{
	    uint8_t buf[256] = { 0 };
	    uint32_t sz = fread(fsnode, 0, 256, buf);
	    kprintf("-r--r--r--\t%s\t\"%s\"\n", node->name, buf);
	}
	i++;
    }

    textmode_color(WHITE, BLACK);
}

static void kernel_halt(void) {
    textmode_color(LIGHTGREEN, BLACK);
    syscall_puts("\n# System halted.........OK");
    halt();
}
