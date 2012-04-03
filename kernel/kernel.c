#include <kernel/kernel.h>

static kernel_arch *kernelArch;

static void kernel_test(void);

void kernel_init(kernel_arch *arch)
{
    /* Setup the screen (by clearing it) */
    vga_device_init();
    puts("# Screen (text mode)\tOK\n");

    kernelArch = arch;
    if (!kernelArch)
	PANIC("No registered architecture.");

    if (!strlen(kernelArch->name))
	PANIC("Registered architecture has no name.");
    else
	kprintf("# Arch: %s\t\tOK\n", kernelArch->name);

    mboot_init(kernelArch->magic, kernelArch->mboot);

    /* Setup architecture-dep. stuff */
    if (kernelArch->setup)
	kernelArch->setup();
    puts("# GDT & IDT\t\tOK\n");

    /* Init the system timer: will be a loadable module */
    timer_init();
    puts("# Timer/Clock\t\tOK\n");

    /* Init the keyboard: will be a loadable module */
    keyboard_init();
    puts("# Keyboard (US)\t\tOK\n");

    if (kernelArch->setup_mmu)
	kernelArch->setup_mmu();
    puts("# Paging\t\tOK\n");

    vfs_init();
    puts("# VFS\t\t\tOK\n");

    ramdisk_init(kernelArch->ramdiskc, kernelArch->ramdiskv);
    puts("# Initrd\t\tOK\n");

    symbol_init();
    puts("# Kernel symbols\tOK\n");

#if KERNEL_DEBUG
    kernel_test();
#endif

    /* Setup syscalls */
    syscalls_init();
    syscall_puts("# Syscalls\t\tOK\n");

#if KERNEL_TEST
    /* Setup multitasking */
    tasking_init();
    syscall_puts("# Multitasking\t\tOK\n");

    /* Switch to user mode */
    usermode_init();
    syscall_puts("# Usermode\t\tOK\n");
#endif

    /* Init a demo shell: will be a loadable exec. */
    shell_init();
}

static void kernel_test(void) {
    void *a, *b, *c, *d;
    int8_t temp[1024] = { 0 };

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
    a = (void *) kmalloc(8);
    b = (void *) kmalloc(8);
    c = (void *) kmalloc(8);
    kprintf("a: 0x%x\n", a);
    kprintf("b: 0x%x\n", b);
    kprintf("c: 0x%x\n", c);
    kfree(c);
    kfree(b);
    d = (void *) kmalloc(12);
    kprintf("d: 0x%x\n", d);
    kfree(a);
    kfree(d);

    /* Try to read files from the ramdisk image */
    puts("# Testing initrd.......\n");
    if (vfs_open("/ramdisk/boot/kernel.sym")) {
	vfs_read("/ramdisk/boot/kernel.sym", 0, 21, temp);
	vfs_close("/ramdisk/boot/kernel.sym");
	temp[1023] = 0;
	puts("---> Reading /ramdisk/boot/kernel.sym\n");
	puts(temp);
	memclr(temp, 1024);
	vfs_open("/");
	vfs_read("/", 0, 1024, temp);
	vfs_close("/");
	puts("---> Reading /\n");
	puts(temp);
    } else {
	puts("Couldn't open /ramdisk/boot/kernel.sym\n");
    }
}

void kernel_halt() {
    syscall_puts("\n# System will now halt.\n");
    kernelArch->halt();
}

void kernel_reboot() {
    syscall_puts("\n# System will now reboot.");
    kernelArch->reboot();
}

void kernel_disable_interrupts() {
    kernelArch->disable_interrupts();

}

void kernel_enable_interrupts() {
    kernelArch->enable_interrupts();
}
