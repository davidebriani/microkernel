SOURCES=main.o dt.o isr.o timer.o kb.o heap.o paging.o panic.o oarray.o

CC=gcc
#CFLAGS=-nostdlib -nostdinc -fno-builtin -fno-stack-protector -Wall -Wextra -I../include
CFLAGS=-nostdlib -nostdinc -fno-builtin -fno-stack-protector -Wall -Wextra -I../include -nostartfiles -nodefaultlibs -ffreestanding -fstrength-reduce -finline-functions -fomit-frame-pointer

all: assemble $(SOURCES)

clean:
	@rm *.o
	@cd asm && make clean

assemble:
	cd asm && make
