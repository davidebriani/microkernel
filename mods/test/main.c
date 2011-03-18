#include <kernel/stdint.h>
#include <kernel/log.h>
#include <kernel/vfs.h>

void init(void);
void destroy(void);

void init(void) {
    int8_t *buffer[1024] = { 0 };
    if (vfs_open("/ramdisk/boot/kernel.sym")) {
	vfs_read("/ramdisk/boot/kernel.sym", 0, 0x100, buffer);
	vfs_close("/ramdisk/boot/kernel.sym");
    }
    buffer[1023] = 0;

    log_write("Testing module loading...\n%s\n", buffer);
}

void destroy(void) {

}
