CROSS_DIR=/usr/cross/i586-elf/bin
GRUB_DIR=/usr/cross/utils/grub
GRUB_FILES=$(GRUB_DIR)/stage1 $(GRUB_DIR)/stage2

GCC=$(CROSS_DIR)/gcc
LD=$(CROSS_DIR)/ld
NASM=/opt/local/bin/nasm

LOADER_SRC=loader.s
KERNEL_SRC=kernel.c
LOADER_OBJ=loader.o
KERNEL_OBJ=kernel.o
KERNEL=kernel.bin
LINKER_SCRIPT=linker.ld

all: img

clean:
	rm *.o pad *.img *.bin
	
assemble:
	$(NASM) -f elf -o $(LOADER_OBJ) $(LOADER_SRC)

compile: assemble
	$(GCC) -o $(KERNEL_OBJ) -c $(KERNEL_SRC) \
		-Wall -Wextra -Werror -nostdlib -nostartfiles -nodefaultlibs
	
link: compile
	$(LD) -T $(LINKER_SCRIPT) -o $(KERNEL) $(LOADER_OBJ) $(KERNEL_OBJ)

img: link
	dd if=/dev/zero of=pad bs=750 count=1
	cat $(GRUB_FILES) pad $(KERNEL) > floppy.img

show_versions:
	$(GCC) -v
	$(LD) -v
	$(NASM) -v