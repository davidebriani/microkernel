#ifndef KERNEL_VFS_RAMDISK_H
#define KERNEL_VFS_RAMDISK_H

#include <kernel/stdint.h>

void vfs_ramdisk_init(struct ramdisk_node *n, uint32_t c);

#endif

