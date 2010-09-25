CC=gcc
CFLAGS=-Wall -Wextra -ansi -O2

all:
	$(CC) $(CFLAGS) -o make_initrd make_initrd.c
	./make_initrd sample.txt program.c
	mv initrd.img ../../

clean:
	@rm -f make_initrd
