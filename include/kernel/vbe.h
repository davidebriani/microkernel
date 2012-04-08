#ifndef KERNEL_ARCH_X86_VBE_H
#define KERNEL_ARCH_X86_VBE_H

#include <kernel/stdint.h>

struct vbe_controller_info {
    int8_t signature[4];
    int16_t version;
    int16_t oem[2];
    uint32_t capabilities[4];
    int16_t modes[2];
    int16_t memory;
};

struct vbe_mode_info {
    int16_t attributes;
    int8_t winA;
    int8_t winB;
    int16_t granularity;
    int16_t winSize;
    int16_t segmentA;
    int16_t segmentB;
};

#endif
