#include "textmode.h"
#include "dt.h"
#include "paging.h"
#include "timer.h"
#include "kb.h"
#include "heap.h"
#include "system.h"
#include "vfs.h"
#include "multiboot.h"
#include "initrd.h"
#include "panic.h"
#include "task.h"

extern placement_address;
uint32_t initial_esp;

int main(struct multiboot *mboot_ptr, uint32_t initial_stack)
{
    initial_esp = initial_stack;

    /* Setup the screen (by clearing it) */
    init_video();
    set_textcolor(LIGHTGREEN, BLACK);
    puts("# Video................ OK\n");

    /* Setup all the ISRs and segmentation */
    init_dt();
    puts("# GDT.................. OK\n");
    puts("# IDT.................. OK\n");

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

    /* Setup paging */
    init_paging();
    puts("# Paging............... OK\n");

    /* Initialise the initial ramdisk, and set it as the filesystem root */
    fs_root = initialise_initrd(initrd_location);

    /* Setup multitasking */
    init_tasking();
    puts("# Multitasking......... OK\n");


    set_textcolor(LIGHTRED, BLACK);

    /* Check if the system really works */
    puts("\n# Testing interrupts...\n");
    puts("\n");
    asm volatile("int $0x0");
    asm volatile("int $0x3");
    /* Important! Re-enable interrupt requests */
    sti();

    /* Check if something went wrong with paging and heap setup */
    puts("\n# Testing the heap.....\n");
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
    puts("\n# Checking initrd.img....\n");
    /* list the contents of / */
    int32_t i = 0;
    struct dirent *node = 0;
    while ( (node = readdir_fs(fs_root, i)) != 0)
    {
	kprintf("Found file %s", node->name);
	fs_node_t *fsnode = finddir_fs(fs_root, node->name);
	if ((fsnode->flags & 0x7) == FS_DIRECTORY)
	    puts("\n\t(directory)\n");
	else
	{
	    puts("\n\tcontents: \"");
	    int8_t buf[256];
	    uint32_t sz = read_fs(fsnode, 0, 256, buf);
	    int32_t j;
	    for (j = 0; j < sz; j++)
		putch(buf[j]);
	    puts("\"\n");
	}
	i++;
    }

    /* Create a new process in an new address space which is a clone of this */
    puts("\n# Testing multitasking...\n");

    /* Fork! */
    int32_t ret = fork();

    /* Print some infos */
    kprintf("fork() = %d\t getpid() = %d\n", ret, getpid());

    /* Wait 5 seconds */
    for (i = 5; i > 0; i--) {
	if (!ret)
	    kprintf("\n%d...\n", i);
	timer_wait(1);
    }

    if (!ret)
	for(;;)putch('>');
    for(;;)putch('<');


    return 0;
}
