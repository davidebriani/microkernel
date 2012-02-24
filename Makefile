# object files from /src/asm go first when linking!
SRCFILES := $(shell find -L src -type f -name "*.c")
ASMFILES := $(shell find -L src -type f -name "*.s")
OBJFILES := $(patsubst %.s,%.o,$(ASMFILES)) $(patsubst %.c,%.o,$(SRCFILES))
LDFLAGS := -Tlink.ld

all: link

clean:
	-@rm kernel initrd.img
	-@cd src && make clean
	-@cd utils/initrd && make clean

link: compile $(OBJFILES)
	ld $(LDFLAGS) -o kernel $(OBJFILES)

compile:
	cd src && make
	cd utils/initrd && make

run: floppy.img
	qemu -soundhw pcspk -fda floppy.img

image: kernel floppy.img initrd.img
	sudo /sbin/losetup /dev/loop0 floppy.img
	sudo mount /dev/loop0 /mnt
	sudo cp kernel /mnt/kernel
	sudo cp initrd.img /mnt/initrd
	sudo cp utils/menu.lst /mnt/boot/grub/
	sudo umount /dev/loop0
	sudo /sbin/losetup -d /dev/loop0
