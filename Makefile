SOURCES=src/asm/boot.o src/main.o src/video/textmode.o src/lib/string.o src/lib/ports.o src/dt.o src/isr.o src/asm/interrupt.o src/asm/dt.o src/timer.o src/kb.o src/heap.o src/paging.o src/panic.o src/oarray.o
LDFLAGS=-Tlink.ld

all: link

clean:
	@rm kernel
	@cd src && make clean

link: compile $(SOURCES)
	ld $(LDFLAGS) -o kernel $(SOURCES)

compile:
	cd src && make

run: floppy.img
	qemu -fda floppy.img

image: kernel floppy.img
	sudo /sbin/losetup /dev/loop0 floppy.img
	sudo mount /dev/loop0 /mnt
	sudo cp kernel /mnt/kernel
	sudo umount /dev/loop0
	sudo /sbin/losetup -d /dev/loop0
