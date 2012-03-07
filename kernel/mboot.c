#include <kernel/mboot.h>
#include <kernel/panic.h>

void mboot_init(uint32_t magic, multiboot_header_t *header) {
    if (magic != MULTIBOOT_MAGIC)
	PANIC("Not a multiboot-compliant boot loader.");
}
