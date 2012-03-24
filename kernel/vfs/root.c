#include <lib/string.h>
#include <kernel/vfs.h>
#include <kernel/vfs/root.h>

static struct vfs_mount *mounts;
static struct vfs_filesystem filesystem;

static uint32_t open(struct vfs_filesystem *self, uint32_t id) {
    return 1;
}

static uint32_t close(struct vfs_filesystem *self, uint32_t id) {
    return 1;
}

static uint32_t read(struct vfs_filesystem *self, uint32_t id, uint32_t offset, uint32_t count, void *buffer) {
    uint32_t length = 7;
    uint32_t i;

    if (id != 1)
        return 0;

    if (length > count)
	return 0;
    strwrt(buffer, "./\n../\n", length);

    for (i = 0; i < VFS_MOUNT_SLOTS; i++)
    {
	if (!mounts[i].filesystem)
            continue;

	if (mounts[i].filesystem == self)
            continue;

	/* if we have to read too much */
	if (length + strlen(buffer + length) > count)
	    return length; /* return 0 ? */

        strwrt(buffer + length, "%s\n", mounts[i].path + 1);
        length += strlen(buffer + length);
    }

    return length;
}

static uint32_t find(struct vfs_filesystem *self, int8_t *name) {
    return 1;
}

void vfs_root_init(struct vfs_mount *m) {
    mounts = m;

    vfs_filesystem_init(&filesystem, open, close, read, 0, find, 0);
    vfs_mount(&filesystem, "/");
}
