GCC_FLAGS=-Wall -Wextra -nostdlib -nostdinc -nostartfiles -nodefaultlibs -fno-builtin -fno-stack-protector -ffreestanding -fstrength-reduce -fomit-frame-pointer -finline-functions -O2 -I./include
LD_FLAGS=-nostdlib -N -T

GCC=gcc
LD=ld
NASM=nasm

KERNEL=kernel.bin
LOADER_SRC=loader.s
LOADER_OBJ=loader.o

OBJS_KERNEL=kernel/main.o kernel/gdt.o kernel/idt.o kernel/isrs.o kernel/irq.o kernel/timer.o kernel/kb.o kernel/asm/helper.o kernel/asm/gdt.o kernel/asm/idt.o kernel/asm/isrs.o kernel/asm/irqs.o
OBJS_VIDEO=video/textmode.o
OBJS_LIB=lib/string.o lib/ports.o
OBJS=$(LOADER_OBJ) $(OBJS_KERNEL) $(OBJS_VIDEO) $(OBJS_LIB)

SUBDIRS=kernel/ video/ lib/

LINKER_SCRIPT=linker.ld

all: img
	@echo "\nHere is our kernel...\n"
	@ls -l $(KERNEL)

makesub:
	@for dir in $(SUBDIRS); do \
	    make --directory=$$dir; \
	done

assemble: makesub
	$(NASM) -f elf -o $(LOADER_OBJ) $(LOADER_SRC)

compile: assemble

link: compile
	$(LD) $(LD_FLAGS) $(LINKER_SCRIPT) -o $(KERNEL) $(OBJS)

img: link
	@dd if=/dev/zero of=pad bs=1024 count=1000

run:
	qemu -kernel $(KERNEL) -hda pad -m 8

clean:
	@rm -f *.o *.bin pad
	@for dir in $(SUBDIRS); do \
	    make --directory=$$dir clean; \
	done

show_versions:
	$(GCC) -v
	$(LD) -v
	$(NASM) -v
