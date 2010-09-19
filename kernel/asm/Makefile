NASM=nasm

SRC=helper.s dt.s isrs.s irqs.s

all: $(SRC)
	@for sfile in $(SRC); do \
	    $(NASM) -f elf $$sfile; \
	done
clean:
	@rm -f *.o

