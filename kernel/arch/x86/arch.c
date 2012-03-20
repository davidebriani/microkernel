#include <kernel/kernel.h>
#include <kernel/panic.h>
#include <kernel/arch/x86/arch.h>
#include <kernel/arch/x86/cpu.h>
#include <kernel/arch/x86/dt.h>
#include <kernel/arch/x86/io.h>
#include <kernel/arch/x86/isr.h>
#include <kernel/arch/x86/mmu.h>
#include <kernel/mboot.h>

static arch_x86 x86;
uint32_t initial_esp;
extern uint32_t placement_address;

static void arch_x86_reboot(void) {
    uint8_t ready = 0x02;

    cpu_disable_interrupts();

    while (ready & 0x02)
        ready = io_inb(0x64);
    io_outb(0x64, 0xFE);
}

static void arch_x86_setup(void) {
    initial_esp = (uint32_t) x86.stack;

    /* Setup all the ISRs and segmentation */
    dt_init();

    /* Find the location of our initial ramdisk which should have
    *  been loaded as a module by the boot loader */
    ASSERT(x86.ramdiskc > 0);
    /* Don't trample our module with placement accesses, please! */
    placement_address = *(uint32_t **)(x86.ramdiskv + 1); /* initrd end */

    cpu_enable_interrupts();
}

void arch_init(uint32_t magic, multiboot_header_t *header, void *stack) {
    x86.setup = arch_x86_setup;
    x86.setup_mmu = mmu_init;
    x86.reboot = arch_x86_reboot;
    x86.halt = cpu_halt;
    x86.enable_interrupts = cpu_enable_interrupts;
    x86.disable_interrupts = cpu_disable_interrupts;
    x86.enter_usermode = cpu_enter_usermode;
    x86.stack = stack;
    x86.set_stack = 0;
    x86.ramdiskc = header->modules.count;
    x86.ramdiskv = header->modules.address;
    x86.mboot = header;
    x86.magic = magic;

    kernel_init(&x86);
}
