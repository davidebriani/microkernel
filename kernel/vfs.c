#include <lib/string.h>
#include <kernel/vfs.h>
#include <kernel/vfs/root.h>

static struct vfs_mount mounts[VFS_MOUNT_SLOTS];

struct vfs_mount *vfs_find_mount(const int8_t *path) {
    uint32_t i, length, max = 0;
    struct vfs_mount *current = 0;

    if (!path)
	return 0;

    for (i = 0; i < VFS_MOUNT_SLOTS; i++) {
        if (!mounts[i].filesystem)
            continue;

        length = strlen(mounts[i].path);

        if (memcmp(mounts[i].path, path, length))
            continue;

        if (length < max)
            continue;

        current = &mounts[i];
        max = length;
    }

    return current;
}

void vfs_mount(struct vfs_filesystem *filesystem, const int8_t *path) {
    uint32_t i;

    for (i = 0; i < VFS_MOUNT_SLOTS; i++) {
        if (mounts[i].filesystem)
            continue;
        vfs_mount_init(&mounts[i], filesystem, path);
        break;
    }
}

void vfs_filesystem_init(struct vfs_filesystem *filesystem, uint32_t (*open)(struct vfs_filesystem *self, uint32_t id), uint32_t (*close)(struct vfs_filesystem *self, uint32_t id), uint32_t (*read)(struct vfs_filesystem *self, uint32_t id, uint32_t offset, uint32_t count, void *buffer), uint32_t (*write)(struct vfs_filesystem *self, uint32_t id, uint32_t offset, uint32_t count, void *buffer), uint32_t (*find)(struct vfs_filesystem *self, const int8_t *path), void *(*get_physical)(struct vfs_filesystem *self, uint32_t id)) {
    memclr(filesystem, sizeof (struct vfs_filesystem));
    filesystem->open = open;
    filesystem->close = close;
    filesystem->read = read;
    filesystem->write = write;
    filesystem->find = find;
    filesystem->get_physical = get_physical;
}

void vfs_mount_init(struct vfs_mount *mount, struct vfs_filesystem *filesystem, const int8_t *path) {
    memclr(mount, sizeof (struct vfs_mount));
    mount->filesystem = filesystem;
    mount->path = path;
}

void vfs_init(void) {
    vfs_root_init(mounts);
}

static uint32_t vfs_find(const int8_t *path, struct vfs_mount **mount) {
    uint32_t id;

    *mount = vfs_find_mount(path);
    if (!*mount)
	return 0;

    if ((*mount)->filesystem->find)
	id = (*mount)->filesystem->find((*mount)->filesystem, path + strlen((*mount)->path));
    else
	return 0;

    return id;
}

uint32_t vfs_open(const int8_t *path) {
    uint32_t id, ret;
    struct vfs_mount *mount;

    id = vfs_find(path, &mount);
    if (!id)
	return 0;

    if (mount->filesystem->open)
	ret = mount->filesystem->open(mount->filesystem, id);
    else
	return 0;

    return ret;
}

uint32_t vfs_close(const int8_t *path) {
    uint32_t id, ret;
    struct vfs_mount *mount;

    id = vfs_find(path, &mount);
    if (!id)
	return 0;

    if (mount->filesystem->close)
	ret = mount->filesystem->close(mount->filesystem, id);
    else
	return 0;

    return ret;
}

uint32_t vfs_read(const int8_t *path, uint32_t offset, uint32_t count, void *buffer) {
    uint32_t id;
    struct vfs_mount *mount;

    id = vfs_find(path, &mount);
    if (!id)
	return 0;

    if (mount->filesystem->read)
	count = mount->filesystem->read(mount->filesystem, id, offset, count, buffer);
    else
	return 0;

    return count;
}

uint32_t vfs_write(const int8_t *path, uint32_t offset, uint32_t count, void *buffer) {
    uint32_t id;
    struct vfs_mount *mount;

    id = vfs_find(path, &mount);
    if (!id)
	return 0;

    if (mount->filesystem->write)
	count = mount->filesystem->write(mount->filesystem, id, offset, count, buffer);
    else
	return 0;

    return count;
}

void *vfs_get_physical(const int8_t *path) {
    uint32_t id;
    struct vfs_mount *mount;
    void *physical;

    id = vfs_find(path, &mount);
    if (!id)
	return 0;

    if (mount->filesystem->get_physical)
	physical = mount->filesystem->get_physical(mount->filesystem, id);
    else
	return 0;

    return physical;
}
