#ifndef KERNEL_KERNEL_H
#define KERNEL_KERNEL_H

#include <kernel/stdint.h>
#include <kernel/mboot.h>
#include <kernel/panic.h>
#include <kernel/timer.h>
#include <kernel/audio/speaker.h>
#include <kernel/vfs.h>
#include <kernel/ramdisk.h>
#include <kernel/symbol.h>
#include <kernel/video/vga.h>
#include <kernel/kb.h>
#include <kernel/heap.h>
#include <kernel/arch/x86/task.h>
#include <kernel/syscall.h>
#include <kernel/shell.h>
#include <kernel/lib/string.h>

#define KERNEL_DEBUG 0
#define KERNEL_TEST  0

typedef struct kernel_arch {
    const int8_t *name;
    void (*setup)(void);
    void (*setup_mmu)(void);
    void (*enable_interrupts)(void);
    void (*disable_interrupts)(void);
    void (*set_stack)(void *address);
    void (*enter_usermode)(uint32_t ip, uint32_t sp);
    void (*reboot)(void);
    void (*halt)(void);
    void *stack;
    uint32_t ramdiskc;
    void **ramdiskv;
    multiboot_header_t *mboot;
    uint32_t magic;
} kernel_arch;

void kernel_disable_interrupts(void);
void kernel_enable_interrupts(void);
void kernel_reboot(void);
void kernel_halt(void);
void kernel_init(kernel_arch *arch);

#endif
