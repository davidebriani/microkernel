#ifndef LIB_TAR_H
#define LIB_TAR_H

#include <kernel/stdint.h>

#define TAR_BLOCK_SIZE 512
#define TAR_MAGIC "ustar"

#define TAR_NAME_SIZE     100
#define TAR_MODE_SIZE     8
#define TAR_UID_SIZE      8
#define TAR_GID_SIZE      8
#define TAR_SIZE_SIZE     12
#define TAR_MTIME_SIZE    12
#define TAR_CHECKSUM_SIZE 8
#define TAR_TYPEFLAG_SIZE 1
#define TAR_MAGIC_SIZE    6
#define TAR_VERSION_SIZE  2
#define TAR_UNAME_SIZE    32
#define TAR_GNAME_SIZE    32
#define TAR_DEVMAJOR_SIZE 8
#define TAR_DEVMINOR_SIZE 8
#define TAR_PREFIX_SIZE   155
#define TAR_FILL_SIZE     12

#define TAR_FILETYPE_NORMAL  '0'
#define TAR_FILETYPE_LINK    '1'
#define TAR_FILETYPE_SYMLINK '2'
#define TAR_FILETYPE_CHAR    '3'
#define TAR_FILETYPE_BLOCK   '4'
#define TAR_FILETYPE_DIR     '5'
#define TAR_FILETYPE_FIFO    '6'
#define TAR_FILETYPE_CONT    '7'

struct tar_header {
    int8_t name[TAR_NAME_SIZE];
    int8_t mode[TAR_MODE_SIZE];
    int8_t uid[TAR_UID_SIZE];
    int8_t gid[TAR_GID_SIZE];
    int8_t size[TAR_SIZE_SIZE];
    int8_t mtime[TAR_MTIME_SIZE];
    int8_t chksum[TAR_CHECKSUM_SIZE];
    int8_t typeflag[TAR_TYPEFLAG_SIZE];
    int8_t linkname[TAR_NAME_SIZE];
    int8_t magic[TAR_MAGIC_SIZE];
    int8_t version[TAR_VERSION_SIZE];
    int8_t uname[TAR_UNAME_SIZE];
    int8_t gname[TAR_GNAME_SIZE];
    int8_t devmajor[TAR_DEVMAJOR_SIZE];
    int8_t devminor[TAR_DEVMINOR_SIZE];
    int8_t prefix[TAR_PREFIX_SIZE];
    int8_t fill[TAR_FILL_SIZE];
};

#endif

