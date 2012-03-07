#ifndef KERNEL_HEAP_H
#define KERNEL_HEAP_H

#include <kernel/stdint.h>
#include <kernel/lib/oarray.h>

#define KHEAP_START		0xC0000000
#define KHEAP_INITIAL_SIZE	0x100000

#define HEAP_INDEX_SIZE		0x20000
#define HEAP_MAGIC		0x123890AB
#define HEAP_MIN_SIZE		0x70000

/* Size information for a hole/block */
typedef struct
{
    uint32_t magic;	/* Magic number, used for error checking and identification */
    uint8_t is_hole;	/* 1 if this is a hole. 0 if this is a block */
    uint32_t size;	/* size of the block, including the end footer */
} header_t;

typedef struct
{
    uint32_t magic;	/* Magic number, same as in header_t */
    header_t *header;	/* Pointer to the block header */
} footer_t;

typedef struct
{
    ordered_array_t index;
    uint32_t start_address;	/* The start of our allocated space */
    uint32_t end_address;	/* The end of our allocated space */
    uint32_t max_address;	/* The maximum address the heap can be expanded to */
    uint8_t supervisor;		/* Should extra pages requested by us be mapped as s.visor-only? */
    uint8_t readonly;		/* Should extra pages requested by us be mapped as read-only? */
} heap_t;

/* Create a new heap */
heap_t *create_heap(uint32_t start, uint32_t end, uint32_t max, uint8_t supervisor, uint8_t readonly);

/* Allocates a contiguous region of memory 'size' in size.
*  If page_align==1, it creates that block starting on a
*  page boundary. */
void *alloc(uint32_t size, uint8_t page_align, heap_t *heap);

/* Releases a block allocated with 'alloc' */
void free(void *p, heap_t *heap);

/* Allocate a chunk of memory, sz in size. If align == 1,
*  the chunk must be page-aligned. If phys != 0, the physical
*  location of the allocated chunk will be stored into phys.
*
*  This is the internal version of kmalloc. More user-friendly
*  parameter representations are available in kmalloc, kmalloc_a,
*  kmalloc_ap, kmalloc_p.
*
*  How it works:
*  As we're allocating quite early on in the kernel bootup,
*  we can make the assumption that nothing that is kmalloc()'d
*  will ever need to be kfree()'d. We can just have a pointer
*  (placement address) to some free memory that we pass back
*  to the requestee then increment. */
uint32_t kmalloc_int(uint32_t sz, int32_t align, uint32_t *phys);

/* Some useful wrapper functions */
uint32_t kmalloc_a(uint32_t sz);		  /* page aligned */
uint32_t kmalloc_p(uint32_t sz, uint32_t *phys);  /* returns a physical address */
uint32_t kmalloc_ap(uint32_t sz, uint32_t *phys); /* page aligned and returns a physical address */
uint32_t kmalloc(uint32_t sz);			  /* normal */

/* General deallocation function */
void kfree(void *p);

#endif
