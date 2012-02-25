#include "paging.h"
#include "heap.h"
#include "string.h"
#include "textmode.h"
#include "panic.h"

/* The kernel's page directory */
page_directory_t *kernel_directory = 0;

/* The current page directory */
page_directory_t *current_directory = 0;

/* A bitset of frames - used or free. */
uint32_t *frames;
uint32_t nframes;

/* Defined in heap.c */
extern uint32_t placement_address;
extern heap_t *kheap;

/* Macros used in the bitset algorithms. */
#define INDEX_FROM_BIT(a)	(a/(8*4))
#define OFFSET_FROM_BIT(a)	(a%(8*4))

/* Static function to set a bit in the frames bitset */
static void set_frame(uint32_t frame_addr)
{
    uint32_t frame, idx, off;

    frame = frame_addr/0x1000;
    idx = INDEX_FROM_BIT(frame);
    off = OFFSET_FROM_BIT(frame);
    frames[idx] |= (0x1 << off);
}

/* Static function to clear a bit in the frames bitset */
static void clear_frame(uint32_t frame_addr)
{
    uint32_t frame, idx, off;

    frame = frame_addr/0x1000;
    idx = INDEX_FROM_BIT(frame);
    off = OFFSET_FROM_BIT(frame);
    frames[idx] &= ~(0x1 << off);
}

/* Static function to test if a bit is set. */
static uint32_t test_frame(uint32_t frame_addr)
{
    uint32_t frame, idx, off;

    frame = frame_addr/0x1000;
    idx = INDEX_FROM_BIT(frame);
    off = OFFSET_FROM_BIT(frame);
    return (frames[idx] & (0x1 << off));
}

/* Static function to find the first free frame. */
static uint32_t first_frame(void)
{
    uint32_t i, j, toTest;

    for (i = 0; i < INDEX_FROM_BIT(nframes); i++)
	if (frames[i] != 0xFFFFFFFF)	/* nothing free, exit early. */
	    for (j = 0; j < 32; j++)	/* at least one bit is free here */
	    {
		toTest = 0x1 << j;
		if ( !(frames[i]&toTest) )
		    return i*4*8+j;
	    }
    return 0;
}

/* Function to allocate a frame. */
void alloc_frame(page_t *page, int32_t is_kernel, int32_t is_writeable)
{
    uint32_t idx;

    if (page->frame != 0)
       return;			/* Frame was already allocated, return straight away. */
    else
    {
	idx = first_frame();	/* idx is now the index of the first free frame. */
	if (idx == (uint32_t) - 1)
	    PANIC("No free frames!");
	set_frame(idx * 0x1000);		/* this frame is now ours! */
	page->present = 1;			/* Mark it as present. */
	page->rw = (is_writeable) ? 1 : 0;	/* Should the page be writeable? */
	page->user = (is_kernel)  ? 0 : 1;	/* Should the page be user-mode? */
	page->frame = idx;
    }
}

/* Function to deallocate a frame. */
void free_frame(page_t *page)
{
    uint32_t frame;

    if (!(frame = page->frame))
	return; /* The given page didn't actually have an allocated frame! */
    else
    {
	clear_frame(frame);	/* Frame is now free again. */
	page->frame = 0x0;	/* Page now doesn't have a frame. */
    }
}

void init_paging()
{
    /* The size of physical memory. For the moment we
    *  assume it is 16MB big. */
    uint32_t mem_end_page = 0x1000000;
    uint32_t i = 0;

    nframes = mem_end_page / 0x1000;
    frames = (uint32_t *) kmalloc(INDEX_FROM_BIT(nframes));
    memset(frames, 0, INDEX_FROM_BIT(nframes));

    /* Let's make a page directory */
    kernel_directory = (page_directory_t *) kmalloc_a(sizeof(page_directory_t));
    memset(kernel_directory, 0, sizeof(page_directory_t));
    kernel_directory->physicalAddr = (uint32_t) kernel_directory->tablesPhysical;

    /* Map some pages in the kernel heap area.
    *  Here we call get_page but not alloc_frame. This causes page_table_t's
    *  to be created where necessary. We can't allocate frames yet because they
    *  they need to be identity mapped first below, and yet we can't increase
    *  placement_address between identity mapping and enabling the heap! */
    for (i = KHEAP_START; i < KHEAP_START + KHEAP_INITIAL_SIZE; i += 0x1000)
	get_page(i, 1, kernel_directory);

    /* We need to identity map (phys addr = virt addr) from
    *  0x0 to the end of used memory, so we can access this
    *  transparently, as if paging wasn't enabled.
    *  NOTE that we use a while loop here deliberately.
    *  inside the loop body we actually change placement_address
    *  by calling kmalloc(). A while loop causes this to be
    *  computed on-the-fly rather than once at the start.
    *  Allocate a little bit extra so the kernel heap can be
    *  initialised properly */
    i = 0;
    while (i < placement_address + 0x1000)	/* 0x400000 */
    {
	/* Kernel code is readable but not writeable from userspace */
	alloc_frame( get_page(i, 1, kernel_directory), 0, 0);
	i += 0x1000;
    }

    /* Now allocate those pages we mapped earlier */
    for (i = KHEAP_START; i < KHEAP_START + KHEAP_INITIAL_SIZE; i += 0x1000)
	alloc_frame( get_page(i, 1, kernel_directory), 0, 0);

    /* Before we enable paging, we must register our page fault handler */
    register_interrupt_handler(14, page_fault);

    /* Now, enable paging! */
    switch_page_directory(kernel_directory);

    /* Setup the kernel heap */
    kheap = create_heap(KHEAP_START, KHEAP_START + KHEAP_INITIAL_SIZE, 0xCFFFF000, 0, 0);

    current_directory = clone_directory(kernel_directory);
    switch_page_directory(current_directory);
}

void switch_page_directory(page_directory_t *dir)
{
    uint32_t cr0;
    current_directory = dir;
    __asm__ __volatile__("mov %0, %%cr3":: "r"(dir->physicalAddr));
    __asm__ __volatile__("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000;	/* Enable paging! */
    __asm__ __volatile__("mov %0, %%cr0":: "r"(cr0));
}

page_t *get_page(uint32_t address, int32_t make, page_directory_t *dir)
{
    uint32_t table_idx, tmp;

    /* Turn the address into an index */
    address /= 0x1000;
    /* Find the page table containing this address */
    table_idx = address / 1024;
    if (dir->tables[table_idx])		/* If this table is already assigned */
    {
	return &dir->tables[table_idx]->pages[address%1024];
    }
    else if(make)
    {
	dir->tables[table_idx] = (page_table_t*)kmalloc_ap(sizeof(page_table_t), &tmp);
	memset(dir->tables[table_idx], 0, 0x1000);
	dir->tablesPhysical[table_idx] = tmp | 0x7;	/* PRESENT, RW, US. */
	return &dir->tables[table_idx]->pages[address%1024];
    }
    else
    {
	return 0;
    }
}

void page_fault(registers_t *regs)
{
    /* A page fault has occurred */
    /* The faulting address is stored in the CR2 register */
    uint32_t faulting_address;
    int32_t present, rw, us, reserved, id;
    __asm__ __volatile__("mov %%cr2, %0" : "=r" (faulting_address));

    /* The error code gives us details of what happened. */
    present = !(regs->err_code & 0x1);	/* Page not present */
    rw = regs->err_code & 0x2;		/* Write operation? */
    us = regs->err_code & 0x4;		/* Processor was in user-mode? */
    reserved = regs->err_code & 0x8;	/* Overwritten CPU-reserved bits of page entry? */
    id = regs->err_code & 0x10;		/* Caused by an instruction fetch? */

    /* Output an error message */
    kprintf("\nPage fault ( ");
    if (present)	kprintf("present ");
    if (rw)		kprintf("read-only ");
    if (us)		kprintf("user-mode ");
    if (reserved)	kprintf("reserved ");
    kprintf(") at 0x%x; EIP=0x%x!\n", faulting_address, regs->eip);
    PANIC("Page fault");
}

static page_table_t *clone_table(page_table_t *src, uint32_t *physAddr)
{
    int32_t i;

    /* Make a new page table, which is page aligned */
    page_table_t *table = (page_table_t *) kmalloc_ap(sizeof(page_table_t), physAddr);
    /* Ensure that the new table is blank */
    memset(table, 0, sizeof(page_directory_t));

    /* For every entry in the table... */
    for (i = 0; i < 1024; i++)
    {
	/* If the source entry has a frame associated with it... */
	if (src->pages[i].frame)
	{
	    /* Get a new frame */
            alloc_frame(&table->pages[i], 0, 0);
            /* Clone the flags from source to destination */
	    if (src->pages[i].present) table->pages[i].present = 1;
	    if (src->pages[i].rw) table->pages[i].rw = 1;
	    if (src->pages[i].user) table->pages[i].user = 1;
	    if (src->pages[i].accessed) table->pages[i].accessed = 1;
	    if (src->pages[i].dirty) table->pages[i].dirty = 1;
	    /* Physically copy the data across. This function is in process.s */
	    copy_page_physical(src->pages[i].frame*0x1000, table->pages[i].frame*0x1000);
	}
    }
    return table;
}

page_directory_t *clone_directory(page_directory_t *src)
{
    uint32_t phys, offset;
    int32_t i;
    /* Make a new page directory and obtain its physical address */
    page_directory_t *dir = (page_directory_t *) kmalloc_ap(sizeof(page_directory_t), &phys);
    /* Ensure that it is blank */
    memset(dir, 0, sizeof(page_directory_t));

    /* Get the offset of tablesPhysical from the start of the page_directory_t structure */
    offset = (uint32_t) dir->tablesPhysical - (uint32_t) dir;

    /* Then the physical address of dir->tablesPhysical is */
    dir->physicalAddr = phys + offset;

    /* Go through each page table. If the page table is in the kernel directory, do not make a new copy */
    for (i = 0; i < 1024; i++)
    {
	if (!src->tables[i])
	    continue;

	if (kernel_directory->tables[i] == src->tables[i])
	{
	    /* It's in the kernel, so just use the same pointer */
	    dir->tables[i] = src->tables[i];
	    dir->tablesPhysical[i] = src->tablesPhysical[i];
	}
	else
	{
	    /* Copy the table */
	    /* uint32_t phys; */
	    dir->tables[i] = clone_table(src->tables[i], &phys);
	    dir->tablesPhysical[i] = phys | 0x07;
	}
    }
    return dir;
}
