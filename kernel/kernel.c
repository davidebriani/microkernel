#include <kernel/kernel.h>
#include <kernel/fs/vfs.h>

static kernel_arch *kernelArch;

static void kernel_test(void);

void kernel_init(kernel_arch *arch)
{
    /* Setup the screen (by clearing it) */
    vga_device_init();
    puts("# Screen (text mode)... OK\n");

    kernelArch = arch;
    if (!kernelArch)
	PANIC("No registered architecture.");

    mboot_init(kernelArch->magic, kernelArch->mboot);

    /* Setup architecture-dep. stuff */
    if (kernelArch->setup)
	kernelArch->setup();
    puts("# GDT & IDT............ OK\n");

    /* Init the system timer: will be a loadable module */
    timer_init();
    puts("# Timer/Clock.......... OK\n");

    /* Init the keyboard: will be a loadable module */
    keyboard_init();
    puts("# Keyboard (US)........ OK\n");

    if (kernelArch->setup_mmu)
	kernelArch->setup_mmu();
    puts("# Paging............... OK\n");

#if KERNEL_DEBUG
    kernel_test();
#endif

    /* Setup syscalls */
    syscalls_init();
    syscall_puts("# Syscalls............. OK\n");

#if KERNEL_TEST
    /* Setup multitasking */
    tasking_init();
    syscall_puts("# Multitasking......... OK\n");

    /* Switch to user mode */
    usermode_init();
    syscall_puts("# Usermode..............OK\n");
#endif

    /* Init a demo shell: will be a loadable exec. */
    shell_init();

    kernel_halt();
}

static void kernel_test(void) {
    puts("# Testing timer........\n");
    sleep(1);

    puts("# Testing PC speaker...\n");
    beep(3000, 0.3);
    beep(2000, 0.3);
    beep(1000, 0.3);
    beep(3000, 0.3);
    beep(2000, 0.3);
    beep(1000, 0.3);

    /* Check if the system really works */
    puts("# Testing interrupts...\n");
    __asm__ __volatile__("int $0x0");
    __asm__ __volatile__("int $0x3");
    /* Important! Re-enable interrupt requests */
    kernel_enable_interrupts();

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
	    fread(fsnode, 0, 256, buf);
	    kprintf("-r--r--r--\t%s\t\"%s\"\n", node->name, buf);
	}
	i++;
    }
}

void kernel_halt() {
    syscall_puts("\n# System halted.........OK");
    kernelArch->halt();
}

void kernel_reboot() {
    syscall_puts("\n# System rebooting......OK");
    kernelArch->reboot();
}

void kernel_disable_interrupts() {
    kernelArch->disable_interrupts();

}

void kernel_enable_interrupts() {
    kernelArch->enable_interrupts();
}
