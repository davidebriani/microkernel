ROOTDIR := .
include $(ROOTDIR)/common.mk

# object files from /src/asm go first when linking!
SRCFILES := $(shell find -L kernel lib -type f -name "*.c")
ASMFILES := $(shell find -L kernel lib -type f -name "*.s")
OBJFILES := $(patsubst %.s,%.o,$(ASMFILES)) $(patsubst %.c,%.o,$(SRCFILES))
LDFLAGS += -Tlink.ld

.PHONY: help kernel link compile ramdisk floppy cdrom run-floppy run-cdrom bochs-x86-floppy bochs-x86-cdrom qemu-x86-floppy qemu-x86-cdrom clean

# [0] make help		Display callable targets.
help:
	@echo "Available options:"
	@grep -oE "\[[0-9]+\] make\s+.*" [Mm]akefile

# [1] make kernel		Compile and link the kernel into an executable.
kernel: link ramdisk

link: compile $(OBJFILES)
	$(LD) $(LDFLAGS) -o $(KERNEL) $(OBJFILES)

compile:
	cd kernel && make
	cd lib && make
	cd mods && make

ramdisk: $(KERNEL)
	cd ramdisk && make

# [2] make floppy		Create a bootable floppy disk image.
floppy: floppy.img $(KERNEL) $(RAMDISK)
	sudo /sbin/losetup /dev/loop0 floppy.img
	sudo mount /dev/loop0 /mnt
	sudo cp $(KERNEL) /mnt/boot/
	sudo cp $(RAMDISK) /mnt/boot/
	sudo cp utils/grub/menu.lst /mnt/boot/grub/
	sudo cp utils/grub/metro.xpm.gz /mnt/boot/grub/splash.xpm.gz
	sudo umount /dev/loop0
	sudo /sbin/losetup -d /dev/loop0

# [2] make cdrom		Create a bootable cdrom disk image.
cdrom: $(KERNEL) $(RAMDISK)
	cp $(KERNEL) ramdisk/root/boot/
	cp $(RAMDISK) ramdisk/root/boot/
	genisoimage -p "TheWorm" -publisher "TheWorm" -V "$(KERNEL) kernel" -A "Simple microkernel for personal research" -R -b boot/grub/iso9660_stage1_5 -no-emul-boot -boot-load-size 4 -boot-info-table -o $(KERNEL).iso ramdisk/root

# [3] make run-floppy	Boot the floppy image on the VM emulator.
run-floppy: floppy.img $(VM)-$(ARCH)-floppy

# [3] make run-cdrom	Boot the cdrom image on the VM emulator.
run-cdrom: $(VM)-$(ARCH)-cdrom

bochs-x86-floppy:
	bochs -qf utils/bochsrc.txt

bochs-x86-cdrom:
	echo "Couldn't run bochs with cdrom..."

qemu-x86-floppy:
	qemu -cpu 486 -smp 1,cores=1,threads=1 -m 32 -k it -soundhw pcspk -fda floppy.img >qemu.log

qemu-x86-cdrom:
	qemu -cpu 486 -smp 1,cores=1,threads=1 -m 32 -k it -soundhw pcspk -cdrom $(KERNEL).iso >qemu.log

# [4] make clean		Clean the environment and remove compiled object files.
clean:
	-@rm -f $(KERNEL) $(RAMDISK) *.log ramdisk/root/boot/$(KERNEL) ramdisk/root/boot/$(RAMDISK) $(KERNEL).iso
	-@cd kernel && make clean
	-@cd lib && make clean
	-@cd mods && make clean
	-@cd ramdisk && make clean
