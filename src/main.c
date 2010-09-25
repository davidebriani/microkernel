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

extern placement_address;

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



    /* Initialise the initial ramdisk, and set it as the filesystem root. */
    fs_root = initialise_initrd(initrd_location);



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
    /* Important! Re-enable interrupt requests */
    sti();

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
	    kprintf("\n\t(directory)\n");
	else
	{
	    kprintf("\n\tcontents: \"");
	    int8_t buf[256];
	    uint32_t sz = read_fs(fsnode, 0, 256, buf);
	    int32_t j;
	    for (j = 0; j < sz; j++)
		putch(buf[j]);
	    kprintf("\"\n");
	}
	i++;
    }

    /* Wait 1 second... */
    timer_wait(1);

    /* Print out something and change text colors */
    set_textcolor(LIGHTGREEN, BLACK);
    puts("\n\n# Aaaalright. Let's rock!\n\n");
    set_textcolor(WHITE, BLACK);


    return 0;
}
