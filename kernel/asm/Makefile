NASM=nasm

SRC=helper.s gdt.s idt.s isrs.s irqs.s

all: $(SRC)
	@for sfile in $(SRC); do \
	    $(NASM) -f elf $$sfile; \
	done
clean:
	@rm -f *.o

