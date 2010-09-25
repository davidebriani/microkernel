SOURCES=src/asm/boot.o src/main.o src/video/textmode.o src/lib/string.o src/lib/ports.o src/dt.o src/isr.o src/asm/interrupt.o src/asm/dt.o src/timer.o src/kb.o src/heap.o src/paging.o src/panic.o src/oarray.o src/initrd.o src/fs/vfs.o
LDFLAGS=-Tlink.ld

all: link

clean:
	@rm kernel initrd.img
	@cd src && make clean
	@cd scripts/initrd && make clean

link: compile $(SOURCES)
	ld $(LDFLAGS) -o kernel $(SOURCES)

compile:
	cd src && make
	cd scripts/initrd && make

run: floppy.img
	qemu -fda floppy.img

image: kernel floppy.img initrd.img
	sudo /sbin/losetup /dev/loop0 floppy.img
	sudo mount /dev/loop0 /mnt
	sudo cp kernel /mnt/kernel
	sudo cp initrd.img /mnt/initrd
	sudo umount /dev/loop0
	sudo /sbin/losetup -d /dev/loop0
