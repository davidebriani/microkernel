ROOTDIR := .
include $(ROOTDIR)/common.mk

# object files from /src/asm go first when linking!
SRCFILES := $(shell find -L kernel lib -type f -name "*.c")
ASMFILES := $(shell find -L kernel lib -type f -name "*.s")
OBJFILES := $(patsubst %.s,%.o,$(ASMFILES)) $(patsubst %.c,%.o,$(SRCFILES))
LDFLAGS += -Tlink.ld

.PHONY: all clean link compile run image

all: link

clean:
	-@rm microkernel initrd.img
	-@cd kernel && make clean
	-@cd lib && make clean
	-@cd utils/initrd && make clean

link: compile $(OBJFILES)
	$(LD) $(LDFLAGS) -o microkernel $(OBJFILES)

compile:
	cd kernel && make
	cd lib && make
	cd utils/initrd && make

run: floppy.img
	qemu -soundhw pcspk -fda floppy.img

image: kernel floppy.img initrd.img
	sudo /sbin/losetup /dev/loop0 floppy.img
	sudo mount /dev/loop0 /mnt
	sudo cp microkernel /mnt/kernel
	sudo cp initrd.img /mnt/initrd
	sudo cp utils/menu.lst /mnt/boot/grub/
	sudo umount /dev/loop0
	sudo /sbin/losetup -d /dev/loop0
