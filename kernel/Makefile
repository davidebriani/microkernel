GCC_FLAGS=-Wall -Wextra -nostdlib -nostdinc -nostartfiles -nodefaultlibs -fno-builtin -fno-stack-protector -ffreestanding -fstrength-reduce -fomit-frame-pointer -finline-functions -O2 -I../include

GCC=gcc

SRC=main.c gdt.c idt.c isrs.c irq.c timer.c kb.c
SUBDIRS=asm/

all: $(SRC)
	$(GCC) $(GCC_FLAGS) -c $(SRC)
	@for dir in $(SUBDIRS); do \
	    make --directory=$$dir; \
	done

clean:
	@rm -f *.o
	@for dir in $(SUBDIRS); do \
	    make --directory=$$dir clean; \
	done
