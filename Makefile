ROOTDIR := .
include $(ROOTDIR)/common.mk

# object files from /src/asm go first when linking!
SRCFILES := $(shell find -L kernel lib -type f -name "*.c")
ASMFILES := $(shell find -L kernel lib -type f -name "*.s")
OBJFILES := $(patsubst %.s,%.o,$(ASMFILES)) $(patsubst %.c,%.o,$(SRCFILES))
LDFLAGS += -Tlink.ld

.PHONY: all link compile image run qemu

all: link

link: compile $(OBJFILES)
	$(LD) $(LDFLAGS) -o microkernel $(OBJFILES)

compile:
	cd kernel && make
	cd lib && make
	cd build/ramdisk/ && tar -cvf initrd.tar *
	mv build/ramdisk/initrd.tar .

image: kernel floppy.img initrd.tar
	sudo /sbin/losetup /dev/loop0 floppy.img
	sudo mount /dev/loop0 /mnt
	sudo cp microkernel /mnt/boot/kernel
	sudo cp initrd.tar /mnt/boot/initrd.tar
	sudo cp utils/grub/menu.lst /mnt/boot/grub/
	sudo cp utils/grub/metro.xpm.gz /mnt/boot/grub/splash.xpm.gz
	sudo umount /dev/loop0
	sudo /sbin/losetup -d /dev/loop0

clean:
	-@rm -f microkernel initrd.tar *.log
	-@cd kernel && make clean
	-@cd lib && make clean

run: floppy.img $(VM)-$(ARCH)

bochs-x86:
	bochs -qf utils/bochsrc.txt

qemu-x86:
	qemu -cpu 486 -smp 1,cores=1,threads=1 -m 32 -k it -soundhw pcspk -fda floppy.img >qemu.log
