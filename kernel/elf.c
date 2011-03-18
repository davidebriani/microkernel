#include <kernel/elf.h>
#include <kernel/symbol.h>
#include <kernel/lib/string.h>

static struct elf_header *get_header(void *address) {
    struct elf_header *header = (struct elf_header *)address;

    if (header->identify[0] != ELF_IDENTITY_MAGIC0)
        return 0;

    if (header->identify[1] != ELF_IDENTITY_MAGIC1)
        return 0;

    if (header->identify[2] != ELF_IDENTITY_MAGIC2)
        return 0;

    if (header->identify[3] != ELF_IDENTITY_MAGIC3)
        return 0;

    return header;
}

uint32_t elf_get_entry(void *address) {
    struct elf_header *header = get_header(address);

    if (!header)
        return 0;

    return (uint32_t) header->entry;
}

uint32_t elf_get_virtual(void *address) {
    struct elf_program_header *pheader;
    struct elf_header *header = get_header(address);

    if (!header)
        return 0;

    pheader = (struct elf_program_header *)((uint32_t) address + header->phoffset);

    return (uint32_t) pheader->vaddress;
}

uint32_t elf_get_symbol(void *address, const int8_t *name) {
    struct elf_section_header *sheader, *relHeader, *symHeader;
    struct elf_symbol *symTable, *symEntry;
    int8_t *infoTable, *strTable;
    uint32_t i, count;
    struct elf_header *header = get_header(address);

    if (!header)
        return 0;

    sheader = (struct elf_section_header *)((uint32_t) address + header->shoffset);
    relHeader = &sheader[2];
    symHeader = &sheader[relHeader->link];

    symTable = (struct elf_symbol *)((uint32_t) address + symHeader->offset);
    infoTable = (int8_t *)address + sheader[relHeader->info].offset;
    strTable =  (int8_t *)address + sheader[symHeader->link].offset;

    count = symHeader->size / symHeader->esize;

    for (i = 0; i < count; i++) {
        symEntry = &symTable[i];

        if (!memcmp(name, strTable + symEntry->name, strlen(name)))
            return (uint32_t)(infoTable + symEntry->value);
    }

    return 0;
}

void elf_prepare(void *address) {
    uint32_t i;
    struct elf_section_header *sheader;
    struct elf_header *header = get_header(address);

    if (!header)
        return;

    sheader = (struct elf_section_header *)((uint32_t) address + header->shoffset);

    for (i = 0; i < header->shcount; i++)
        if (sheader[i].type == 8)
            memclr((int8_t *)address + sheader[i].offset, sheader[i].size);
}

void elf_relocate(void *address) {
    struct elf_section_header *sheader, *relHeader, *symHeader;
    struct elf_relocate *relTable, *relEntry;
    struct elf_symbol *symTable, *symEntry;
    int8_t *infoTable, *strTable;
    uint32_t i, count, *entry, value, addend;
    uint8_t type, index;
    struct elf_header *header = get_header(address);

    if (!header)
        return;

    sheader = (struct elf_section_header *)((uint32_t) address + header->shoffset);
    relHeader = &sheader[2];
    symHeader = &sheader[relHeader->link];

    relTable = (struct elf_relocate *)((uint32_t) address + relHeader->offset);
    symTable = (struct elf_symbol *)((uint32_t) address + symHeader->offset);
    infoTable = (int8_t *)address + sheader[relHeader->info].offset;
    strTable =  (int8_t *)address + sheader[symHeader->link].offset;

    count = relHeader->size / relHeader->esize;

    for (i = 0; i < count; i++) {
        relEntry = &relTable[i];

        type =  relEntry->info & 0x0F;
        index = relEntry->info >> 8;

        symEntry = &symTable[index];
        entry = (uint32_t *)(infoTable + relEntry->offset);
        value = *entry;
        addend = (symEntry->shindex) ? (uint32_t) address + sheader[symEntry->shindex].offset + symEntry->value : symbol_find(strTable + symEntry->name);

        switch (type) {
            case 1:
                *entry = value + addend;
                break;
            case 2:
                *entry = value + addend - (uint32_t) entry;
                break;
        }
    }
}
