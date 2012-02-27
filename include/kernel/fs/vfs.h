#ifndef VFS_H
#define VFS_H

#include "kernel/stdint.h"

#define FS_FILE		0x01
#define FS_DIRECTORY	0x02
#define FS_CHARDEVICE	0x03
#define FS_BLOCKDEVICE	0x04
#define FS_PIPE		0x05
#define FS_SYMLINK	0x06
#define FS_MOUNTPOINT	0x08	/* Is the file an active mountpoint? */

#define FS_NAME_MAX 128

struct fs_node;

/* These typedefs define the type of callbacks,
*  called when read/write/open/close are called */
typedef uint32_t (*read_type_t)(struct fs_node*, uint32_t, uint32_t, uint8_t *);
typedef uint32_t (*write_type_t)(struct fs_node*, uint32_t, uint32_t, uint8_t *);
typedef void (*open_type_t)(struct fs_node*);
typedef void (*close_type_t)(struct fs_node*);
typedef struct dirent * (*readdir_type_t)(struct fs_node*, uint32_t);
typedef struct fs_node * (*finddir_type_t)(struct fs_node*, int8_t *name);

typedef struct fs_node
{
    int8_t name[FS_NAME_MAX];	/* The filename */
    uint32_t mask;	/* The permissions mask */
    uint32_t uid;	/* The owning user */
    uint32_t gid;	/* The owning group */
    uint32_t flags;	/* Includes the node type. See #defines above */
    uint32_t inode;	/* This is device-specific - provides a way for a filesystem to identify files */
    uint32_t length;	/* Size of the file, in bytes */
    uint32_t impl;	/* An implementation-defined number */
    read_type_t read;
    write_type_t write;
    open_type_t open;
    close_type_t close;
    readdir_type_t readdir;
    finddir_type_t finddir;
    struct fs_node *ptr;	/* Used by mountpoints and symlinks */
} fs_node_t;

struct dirent
{
    int8_t name[FS_NAME_MAX];   /* Filename */
    uint32_t ino;	        /* Inode number. Required by POSIX */
};

extern fs_node_t *fs_root;	/* The root of the filesystem */

/* Standard read/write/open/close functions. Note that these are all suffixed */
uint32_t fread(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer);
uint32_t fwrite(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer);
void fopen(fs_node_t *node, uint8_t read, uint8_t write);
void fclose(fs_node_t *node);
struct dirent *readdir(fs_node_t *node, uint32_t index);
fs_node_t *finddir(fs_node_t *node, int8_t *name);

#endif
