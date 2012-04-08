#include <kernel/symbol.h>
#include <kernel/panic.h>
#include <kernel/vfs.h>
#include <kernel/lib/string.h>

static struct symbol_entry symbol_entries[SYMBOL_ENTRY_SLOTS];
static int8_t symbol_map[SYMBOL_MAP_SIZE];

uint32_t symbol_find(const int8_t *name) {
    uint32_t i;

    for (i = 0; i < SYMBOL_ENTRY_SLOTS; i++)
        if (!memcmp(symbol_entries[i].name, name, strlen(symbol_entries[i].name)))
            return symbol_entries[i].paddress;

    return 0;
}

void symbol_init(void) {
    uint32_t count, i, start = 0, index = 0;

    if (!vfs_open("/ramdisk/boot/kernel.map"))
	PANIC("Symbol table not found.");

    count = vfs_read("/ramdisk/boot/kernel.map", 0, SYMBOL_MAP_SIZE, symbol_map);

    for (i = 0; i < count; i++) {
        switch (symbol_map[i]) {
            case ' ':
                symbol_map[i] = '\0';
                break;
            case '\n':
                symbol_map[i] = '\0';
		/* TODO: I don't like this '+ 11' below */
                strwrt(symbol_entries[index].name, "%s", symbol_map + start + 11);
                symbol_entries[index].paddress = strreadn(symbol_map + start, 16);
                index++;
                start = i + 1;
                break;
        }
    }
}
