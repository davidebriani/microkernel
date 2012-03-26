ROOTDIR := .
include $(ROOTDIR)/common.mk

# object files from /src/asm go first when linking!
SRCFILES := $(shell find -L kernel lib -type f -name "*.c")
ASMFILES := $(shell find -L kernel lib -type f -name "*.s")
OBJFILES := $(patsubst %.s,%.o,$(ASMFILES)) $(patsubst %.c,%.o,$(SRCFILES))
LDFLAGS += -Tlink.ld

.PHONY: all kernel ramdisk compile floppy cdrom run-floppy run-cdrom

all: kernel ramdisk

kernel: compile $(OBJFILES)
	$(LD) $(LDFLAGS) -o $(KERNEL) $(OBJFILES)

compile:
	cd kernel && make
	cd lib && make
	cd mods && make

ramdisk: $(KERNEL)
	cd ramdisk && make

floppy: floppy.img $(KERNEL) $(RAMDISK)
	sudo /sbin/losetup /dev/loop0 floppy.img
	sudo mount /dev/loop0 /mnt
	sudo cp $(KERNEL) /mnt/boot/
	sudo cp $(RAMDISK) /mnt/boot/
	sudo cp utils/grub/menu.lst /mnt/boot/grub/
	sudo cp utils/grub/metro.xpm.gz /mnt/boot/grub/splash.xpm.gz
	sudo umount /dev/loop0
	sudo /sbin/losetup -d /dev/loop0

cdrom: $(KERNEL) $(RAMDISK)
	cp $(KERNEL) ramdisk/root/boot/
	cp $(RAMDISK) ramdisk/root/boot/
	genisoimage -p "TheWorm" -publisher "TheWorm" -V "$(KERNEL) kernel" -A "Simple microkernel for personal research" -R -b boot/grub/iso9660_stage1_5 -no-emul-boot -boot-load-size 4 -boot-info-table -o $(KERNEL).iso ramdisk/root

clean:
	-@rm -f $(KERNEL) $(RAMDISK) *.log ramdisk/root/boot/$(KERNEL) ramdisk/root/boot/$(RAMDISK) $(KERNEL).iso
	-@cd kernel && make clean
	-@cd lib && make clean
	-@cd mods && make clean
	-@cd ramdisk && make clean

run-floppy: floppy.img $(VM)-$(ARCH)-floppy

run-cdrom: $(VM)-$(ARCH)-cdrom

bochs-x86-floppy:
	bochs -qf utils/bochsrc.txt

bochs-x86-cdrom:
	echo "Couldn't run bochs with cdrom..."

qemu-x86-floppy:
	qemu -cpu 486 -smp 1,cores=1,threads=1 -m 32 -k it -soundhw pcspk -fda floppy.img >qemu.log

qemu-x86-cdrom:
	qemu -cpu 486 -smp 1,cores=1,threads=1 -m 32 -k it -soundhw pcspk -cdrom $(KERNEL).iso >qemu.log
