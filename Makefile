ROOTDIR := .
include $(ROOTDIR)/common.mk

# object files from /src/asm go first when linking!
SRCFILES := $(shell find -L kernel lib -type f -name "*.c")
ASMFILES := $(shell find -L kernel lib -type f -name "*.s")
OBJFILES := $(patsubst %.s,%.o,$(ASMFILES)) $(patsubst %.c,%.o,$(SRCFILES))
LDFLAGS += -Tlink.ld

.PHONY: all kernel ramdisk compile floppy run qemu

all: kernel ramdisk

kernel: compile $(OBJFILES)
	$(LD) $(LDFLAGS) -o microkernel $(OBJFILES)

compile:
	cd kernel && make
	cd lib && make
	cd mods && make

ramdisk: microkernel
	cp mods/*.ko build/ramdisk/mods/
	nm microkernel | grep -f build/ramdisk/boot/kernel.sym > build/ramdisk/boot/kernel.map
	cd build/ramdisk/ && tar -cvf initrd.tar *
	mv build/ramdisk/initrd.tar .

floppy: microkernel floppy.img initrd.tar
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
	-@cd mods && make clean

run: floppy.img $(VM)-$(ARCH)

bochs-x86:
	bochs -qf utils/bochsrc.txt

qemu-x86:
	qemu -cpu 486 -smp 1,cores=1,threads=1 -m 32 -k it -soundhw pcspk -fda floppy.img >qemu.log
