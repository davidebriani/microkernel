#include <kernel/lib/string.h>
#include <kernel/lib/tar.h>
#include <kernel/ramdisk.h>
#include <kernel/vfs.h>
#include <kernel/vfs/ramdisk.h>

static struct ramdisk_node *nodes;
static uint32_t nodesCount;
static struct vfs_filesystem filesystem;

static uint32_t open(struct vfs_filesystem *self, uint32_t id) {
    return 1;
}

static uint32_t close(struct vfs_filesystem *self, uint32_t id) {
    return 1;
}

static uint32_t read(struct vfs_filesystem *self, uint32_t id, uint32_t offset, uint32_t count, void *buffer) {
    uint32_t i, length, size, slash;
    const int8_t *start;
    struct ramdisk_node *node = &nodes[id - 1];

    int8_t *in = (int8_t *)buffer;

    if (node->header->typeflag[0] == TAR_FILETYPE_DIR) {
	length = 7;
	if (length > count)
	    return 0;
        strwrt(in, "./\n../\n", length);

        for (i = 0; i < nodesCount; i++) {
            if (&nodes[i] == node)
                continue;
            if (!strfnd(nodes[i].name, node->name))
                continue;

            start = nodes[i].name + strlen(node->name);
            size = strlen(start);
            slash = memidx(start, '/', size);

            if (slash != size && slash != size - 1)
                continue;

	    /* if we have to read too much */
	    if (length + strlen(in + length) > count)
		return length; /* return 0 ? */

            strwrt(in + length, "%s\n", start);
            length += strlen(in + length);
        }
        return length;
    } else {
        if (count + offset > node->size)
            count = node->size - offset;
        memcpy(in, (int8_t *)node->data + offset, count);
        return count;
    }

    return 0;
}

static uint32_t write(struct vfs_filesystem *self, uint32_t id, uint32_t offset, uint32_t count, void *buffer) {
    struct ramdisk_node *node = &nodes[id - 1];

    int8_t *in = (int8_t *)buffer;

    if (node->header->typeflag[0] == TAR_FILETYPE_DIR) {
        return 0;
    } else {
        if (count + offset > node->size)
            count = node->size - offset;
        memcpy((int8_t *)node->data + offset, in, count);
        return count;
    }

    return 0;
}

static uint32_t find(struct vfs_filesystem *self, const int8_t *name) {
    uint32_t i, l2, length = strlen(name);

    if (!length)
        return 1;

    for (i = 0; i < nodesCount; i++) {
        l2 = strlen(nodes[i].name);
        if (length != l2)
            continue;
        if (!memcmp(nodes[i].name, name, length))
            return i + 1;
    }

    return 0;
}

static void *get_physical(struct vfs_filesystem *self, uint32_t id) {
    return nodes[id - 1].data;
}

void vfs_ramdisk_init(struct ramdisk_node *n, uint32_t c) {
    nodes = n;
    nodesCount = c;

    vfs_filesystem_init(&filesystem, open, close, read, write, find, get_physical);
    vfs_mount(&filesystem, "/ramdisk/");
}
