#include <lib/string.h>
#include <kernel/log.h>
#include <kernel/vfs.h>

void log_write(const char *buffer, ...) {
    int8_t temp[0x100] = { 0 };
    void **arg;
    uint32_t count;

    if (!vfs_open("/dev/tty/stdout"))
	return;

    arg = (void **)&buffer + 1;

    strwrt(temp, buffer, *arg);

    count = strlen(temp) + 1;

    vfs_write("/dev/tty/stdout", 0, count, temp);
    vfs_close("/dev/tty/stdout");
}

