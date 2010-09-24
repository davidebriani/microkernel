SOURCES=boot.o src/main.o video/textmode.o lib/string.o lib/ports.o src/dt.o src/isr.o src/asm/interrupt.o src/asm/dt.o src/timer.o src/kb.o src/heap.o src/paging.o src/panic.o src/oarray.o
NASM=nasm

ASFLAGS=-felf
#CFLAGS=-nostdlib -nostdinc -fno-builtin -fno-stack-protector -Iinclude
LDFLAGS=-Tlink.ld

all: link

clean:
	@rm *.o kernel
	@cd src && make clean
	@cd video && make clean
	@cd lib && make clean

link: compile $(SOURCES)
	ld $(LDFLAGS) -o kernel $(SOURCES)

compile: assemble
	cd video && make
	cd src && make
	cd lib && make

assemble:
	$(NASM) $(ASFLAGS) boot.s

run: floppy.img
	qemu -fda floppy.img

image: kernel floppy.img
	sudo /sbin/losetup /dev/loop0 floppy.img
	sudo mount /dev/loop0 /mnt
	sudo cp kernel /mnt/kernel
	sudo umount /dev/loop0
	sudo /sbin/losetup -d /dev/loop0
