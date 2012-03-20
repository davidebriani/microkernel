#ifndef KERNEL_VFS_H
#define KERNEL_VFS_H

#include <kernel/stdint.h>

#define VFS_FILESYSTEM_SLOTS 8
#define VFS_MOUNT_SLOTS 16

struct vfs_filesystem {
    uint32_t (*open)(struct vfs_filesystem *self, uint32_t id);
    uint32_t (*close)(struct vfs_filesystem *self, uint32_t id);
    uint32_t (*read)(struct vfs_filesystem *self, uint32_t id, uint32_t offset, uint32_t count, void *buffer);
    uint32_t (*write)(struct vfs_filesystem *self, uint32_t id, uint32_t offset, uint32_t count, void *buffer);
    uint32_t (*find)(struct vfs_filesystem *self, int8_t *path);
    void *(*get_physical)(struct vfs_filesystem *self, uint32_t id);
};

struct vfs_mount {
    struct vfs_filesystem *filesystem;
    const int8_t *path;
};

struct vfs_mount *vfs_find_mount(const int8_t *path);
void vfs_mount(struct vfs_filesystem *filesystem, const int8_t *path);
void vfs_filesystem_init(struct vfs_filesystem *filesystem, uint32_t (*open)(struct vfs_filesystem *self, uint32_t id), uint32_t (*close)(struct vfs_filesystem *self, uint32_t id), uint32_t (*read)(struct vfs_filesystem *self, uint32_t id, uint32_t offset, uint32_t count, void *buffer), uint32_t (*write)(struct vfs_filesystem *self, uint32_t id, uint32_t offset, uint32_t count, void *buffer), uint32_t (*find_node)(struct vfs_filesystem *self, const int8_t *path), void *(*get_physical)(struct vfs_filesystem *self, uint32_t id));
void vfs_mount_init(struct vfs_mount *mount, struct vfs_filesystem *filesystem, const int8_t *path);
void vfs_init(void);

uint32_t vfs_open(const int8_t *path);
uint32_t vfs_close(const int8_t *path);
uint32_t vfs_read(const int8_t *path, uint32_t offset, uint32_t count, void *buffer);
uint32_t vfs_write(const int8_t *path, uint32_t offset, uint32_t count, void *buffer);

#endif

