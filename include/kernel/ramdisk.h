#ifndef KERNEL_RAMDISK_H
#define KERNEL_RAMDISK_H

#include <kernel/stdint.h>
#include <kernel/lib/tar.h>

#define RAMDISK_NODE_SLOTS 64

struct ramdisk_node {
    const int8_t *name;
    uint32_t size;
    struct tar_header *header;
    void *data;
};

void ramdisk_node_init(struct ramdisk_node *node, int8_t *name, uint32_t size, struct tar_header *header, void *data);
void ramdisk_init(uint32_t ramdiskc, void **ramdiskv);

#endif

