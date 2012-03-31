#include <kernel/lib/string.h>
#include <kernel/lib/tar.h>
#include <kernel/ramdisk.h>
#include <kernel/vfs.h>
#include <kernel/vfs/ramdisk.h>

static struct ramdisk_node nodes[RAMDISK_NODE_SLOTS];

static uint32_t parse(void *address) {
    uint32_t i, size;
    struct tar_header *header;

    /* TODO: Verify !strcmp(header->magic, TAR_MAGIC); */
    for (i = 0; *(int8_t *)address; i++) {
        header = address;
        size = strreadn(header->size, 8);

        ramdisk_node_init(&nodes[i], header->name, size, header, address + TAR_BLOCK_SIZE);

        address += ((size / TAR_BLOCK_SIZE) + 1) * TAR_BLOCK_SIZE;

        if (size % TAR_BLOCK_SIZE)
            address += TAR_BLOCK_SIZE;
    }
    return i;
}

void ramdisk_node_init(struct ramdisk_node *node, int8_t *name, uint32_t size, struct tar_header *header, void *data) {
    memclr(node, sizeof(struct ramdisk_node));
    node->name = name;
    node->size = size;
    node->header = header;
    node->data = data;
}

void ramdisk_init(uint32_t ramdiskc, void **ramdiskv) {
    uint32_t i, count = 0;

    for (i = 0; i < ramdiskc; i++)
        count += parse(*(ramdiskv + i));

    vfs_ramdisk_init(nodes, count);
}

