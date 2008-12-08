CROSS_DIR=/usr/cross/i586-elf/bin
GRUB_DIR=/usr/cross/utils/grub
GRUB_FILES=$(GRUB_DIR)/stage1 $(GRUB_DIR)/stage2
GCC_FLAGS=-Wall -Wextra -nostdlib -nostartfiles \
-nodefaultlibs -fno-builtin -I./

GCC=$(CROSS_DIR)/gcc
LD=$(CROSS_DIR)/ld
NASM=/opt/local/bin/nasm

KERNEL_SRC=kernel.c
KERNEL_OBJ=kernel.o
KERNEL=kernel.bin
LOADER_SRC=loader.s
LOADER_OBJ=loader.o
SCRN_SRC=scrn.c
SCRN_OBJ=scrn.o
LINKER_SCRIPT=linker.ld

all: img
	ls -l $(KERNEL)
	
assemble:
	$(NASM) -f elf -o $(LOADER_OBJ) $(LOADER_SRC)

compile: assemble
	$(GCC) -o $(KERNEL_OBJ) -c $(KERNEL_SRC) $(GCC_FLAGS)
		
	$(GCC) -o $(SCRN_OBJ) -c $(SCRN_SRC) $(GCC_FLAGS)
	
link: compile
	$(LD) -T $(LINKER_SCRIPT) -o $(KERNEL) $(LOADER_OBJ) \
	 	$(KERNEL_OBJ) $(SCRN_OBJ)

img: link
	@dd if=/dev/zero of=pad bs=750 count=1
	@cat $(GRUB_FILES) pad $(KERNEL) > floppy.img

clean:
	@rm *.o pad *.img *.bin

show_versions:
	$(GCC) -v
	$(LD) -v
	$(NASM) -v
	