#ifndef KERNEL_INITRD_H
#define KERNEL_INITRD_H

#include <kernel/stdint.h>
#include <kernel/fs/vfs.h>

typedef struct
{
    uint32_t nfiles;	/* The number of files in the ramdisk */
} initrd_header_t;

typedef struct
{
    uint8_t magic;	/* Magic number, for error checking */
    int8_t name[64];	/* Filename */
    uint32_t offset;	/* Offset in the initrd that the file starts */
    uint32_t length;	/* Length of the file */
} initrd_file_header_t;

/* Initialises the initial ramdisk. It gets passed the address of
*  the multiboot module, and returns a completed filesystem node */
fs_node_t *initrd_init(uint32_t location);

#endif
