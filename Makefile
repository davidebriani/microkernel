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
<<<<<<< HEAD
	$(LD) $(LDFLAGS) -o $(KERNEL) $(OBJFILES)
=======
	$(LD) $(LDFLAGS) -o microkernel $(OBJFILES)
>>>>>>> 83e35da4263f8ee470c5e10b7fc1891fdd3b81f7

compile:
	cd kernel && make
	cd lib && make
	cd mods && make
<<<<<<< HEAD

ramdisk: $(KERNEL)
	cd ramdisk && make

floppy: floppy.img $(KERNEL) $(RAMDISK)
=======

ramdisk: microkernel
	cp mods/*.ko build/ramdisk/mods/
	nm microkernel | grep -f build/ramdisk/boot/kernel.sym > build/ramdisk/boot/kernel.map
	cd build/ramdisk/ && tar -cvf initrd.tar *
	mv build/ramdisk/initrd.tar .

floppy: microkernel floppy.img initrd.tar
>>>>>>> 83e35da4263f8ee470c5e10b7fc1891fdd3b81f7
	sudo /sbin/losetup /dev/loop0 floppy.img
	sudo mount /dev/loop0 /mnt
	sudo cp $(KERNEL) /mnt/boot/
	sudo cp $(RAMDISK) /mnt/boot/
	sudo cp utils/grub/menu.lst /mnt/boot/grub/
	sudo cp utils/grub/metro.xpm.gz /mnt/boot/grub/splash.xpm.gz
	sudo umount /dev/loop0
	sudo /sbin/losetup -d /dev/loop0

clean:
	-@rm -f $(KERNEL) $(RAMDISK) *.log
	-@cd kernel && make clean
	-@cd lib && make clean
	-@cd mods && make clean
<<<<<<< HEAD
	-@cd ramdisk && make clean
=======
>>>>>>> 83e35da4263f8ee470c5e10b7fc1891fdd3b81f7

run: floppy.img $(VM)-$(ARCH)

bochs-x86:
	bochs -qf utils/bochsrc.txt

qemu-x86:
	qemu -cpu 486 -smp 1,cores=1,threads=1 -m 32 -k it -soundhw pcspk -fda floppy.img >qemu.log
