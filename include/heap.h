#ifndef HEAP_H
#define HEAP_H

#include "stdint.h"

/* As we're allocating quite early on in the kernel bootup,
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

#endif
