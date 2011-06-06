SRCFILES := $(shell ls *.c)
OBJFILES := $(patsubst %.c,%.o,$(SRCFILES))
#OBJFILES := main.o dt.o isr.o timer.o kb.o heap.o paging.o panic.o oarray.o initrd.o task.o syscall.o

CC := gcc
WARNINGS := -Wall -Wextra -ansi -pedantic -Wno-unused-parameter -Wshadow -Wpointer-arith -Wcast-align -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations -Wredundant-decls -Wnested-externs -Winline -Wno-long-long -Wuninitialized -Wstrict-prototypes
CFLAGS :=-nostdlib -nostdinc -fno-builtin -fno-stack-protector -I../include -nostartfiles -nodefaultlibs -ffreestanding -fstrength-reduce -finline-functions -fomit-frame-pointer -std=c99 $(WARNINGS)

all: assemble $(OBJFILES)

clean:
	-@rm *.o
	-@cd asm && make clean
	-@cd lib && make clean
	-@cd video && make clean
	-@cd fs && make clean

assemble:
	cd asm && make
	cd lib && make
	cd video && make
	cd fs && make
