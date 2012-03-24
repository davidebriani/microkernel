#include <kernel/log.h>
#include <kernel/vfs.h>

void init(void) {
    char *buffer[1024] = { 0 };
    if (vfs_open("/ramdisk/boot/sample.txt")) {
	vfs_read("/ramdisk/boot/sample.txt", 0, 0x100, buffer);
	vfs_close("/ramdisk/boot/sample.txt");
    }
    buffer[1023] = 0;

    log_write("Testing module loading...\n%s\n", buffer);
}

void destroy(void) {

}
