#include <kernel/mboot.h>
#include <kernel/panic.h>
#include <kernel/log.h>
#include <kernel/vbe.h>
#include <kernel/kernel.h>

void multiboot_init(uint32_t magic, multiboot_header_t *header) {
    uint32_t deviceNumber, total, address;
    struct multiboot_mmap *mmap;
    struct multiboot_module *module;
    struct vbe_controller_info *controller;

    if (!KERNEL_DEBUG)
	return;

    if (magic != MULTIBOOT_MAGIC)
	PANIC("Not a multiboot-compliant boot loader.");

    if (header->flags & MULTIBOOT_FLAG_LOADER)
	log_write("[mboot] Boot loader: %s\n", (int8_t *) header->boot_loader_name);

    if (header->flags & MULTIBOOT_FLAG_DEVICE) {
        deviceNumber = header->boot_device >> 24;

        log_write("[mboot] Boot device: ");

        switch (deviceNumber) {
            case 0xE0:
                log_write("CD\n");
                break;
            case 0x00:
                log_write("Floppy Disk\n");
                break;
            case 0x80:
                log_write("Hard Drive\n");
                break;
            default:
                log_write("Device: Unknown\n");
                break;
        }
    }

    if (header->flags & MULTIBOOT_FLAG_CMDLINE)
        log_write("[mboot] Kernel: %s\n", (int8_t *) header->cmdline);

    if (header->flags & MULTIBOOT_FLAG_AOUT)
        log_write("[mboot] Kernel type: AOUT\n");

    if (header->flags & MULTIBOOT_FLAG_ELF)
        log_write("[mboot] Kernel type: ELF\n");

    if (header->flags & MULTIBOOT_FLAG_MODULES) {
	module = (struct multiboot_module *) *header->modules.address;
        log_write("[mboot] Module address: 0x%x\n", module->base);
    }

    if (header->flags & MULTIBOOT_FLAG_MEMORY) {
        log_write("[mboot] Lower memory: %d KiB\n", header->mem_lower);
        log_write("[mboot] Upper memory: %d KiB\n", header->mem_upper);
    }

    if (header->flags & MULTIBOOT_FLAG_MMAP) {
        address = (uint32_t) header->mmap.address;
        total = address + header->mmap.count;

        while (address < total) {
            mmap = (struct multiboot_mmap *) address;

            if (mmap->type == 1)
                log_write("[mboot] Memory available: 0x%x:0x%x\n", mmap->blow, mmap->llow);
            else
                log_write("[mboot] Memory reserved: 0x%x:0x%x\n", mmap->blow, mmap->llow);

            address += mmap->size + sizeof (uint32_t);
        }

    }

    if (header->flags & MULTIBOOT_FLAG_VBE) {
        log_write("[mboot] VBE ctrl address: 0x%x\n", header->vbe.control_info);

        if (header->vbe.control_info) {
            controller = (struct vbe_controller_info *) header->vbe.control_info;
            log_write("[mboot] VBE ctrl signature: 0x%x\n", controller->version);
        }

        log_write("[mboot] VBE mode address: 0x%x\n", header->vbe.mode_info);
        log_write("[mboot] VBE interface segment: 0x%x\n", header->vbe.interface_segment);
        log_write("[mboot] VBE interface offset: 0x%x\n", header->vbe.interface_offset);
        log_write("[mboot] VBE interface length: 0x%x\n", header->vbe.interface_length);
    }
}
