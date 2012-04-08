#ifndef KERNEL_MULTIBOOT_H
#define KERNEL_MULTIBOOT_H

#include <kernel/stdint.h>

#define MULTIBOOT_MAGIC    0x2BADB002

#define MULTIBOOT_FLAG_MEMORY	1 << 0
#define MULTIBOOT_FLAG_DEVICE	1 << 1
#define MULTIBOOT_FLAG_CMDLINE	1 << 2
#define MULTIBOOT_FLAG_MODULES	1 << 3
#define MULTIBOOT_FLAG_AOUT	1 << 4
#define MULTIBOOT_FLAG_ELF	1 << 5
#define MULTIBOOT_FLAG_MMAP	1 << 6
#define MULTIBOOT_FLAG_CONFIG	1 << 7
#define MULTIBOOT_FLAG_LOADER	1 << 8
#define MULTIBOOT_FLAG_APM	1 << 9
#define MULTIBOOT_FLAG_VBE	1 << 10

struct multiboot_modules_header {
    uint32_t count;
    void **address;
};

struct multiboot_elf_header {
    uint32_t shcount;
    uint32_t shsize;
    void *shaddress;
    uint32_t shstringindex;
};

struct multiboot_mmap_header {
    uint32_t count;
    void *address;
};

struct multiboot_drives_header {
    uint32_t count;
    void *address;
};

struct multiboot_vbe_header {
    uint32_t control_info;
    uint32_t mode_info;
    uint16_t mode;
    uint16_t interface_segment;
    uint16_t interface_offset;
    uint16_t interface_length;
} __attribute__((packed));

struct multiboot_header {
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
    uint32_t boot_device;
    uint32_t cmdline;
    struct multiboot_modules_header modules;
    struct multiboot_elf_header elf;
    struct multiboot_mmap_header mmap;
    struct multiboot_drives_header drives;
    uint32_t config_table;
    uint32_t boot_loader_name;
    uint32_t apm_table;
    struct multiboot_vbe_header vbe;
} __attribute__((packed));

typedef struct multiboot_header multiboot_header_t;

struct multiboot_mmap {
    uint32_t size;
    uint32_t blow;
    uint32_t bhigh;
    uint32_t llow;
    uint32_t lhigh;
    uint32_t type;
};

struct multiboot_module {
    uint32_t base;
    uint32_t length;
    uint32_t name;
    uint32_t reserved;
};

void multiboot_init(uint32_t magic, multiboot_header_t *header);

#endif
