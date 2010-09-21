NASM=nasm
ASFLAGS=-felf

SRC=dt.s interrupt.s

all: $(SRC)
	for sfile in $(SRC); do \
	    $(NASM) $(ASFLAGS) $$sfile; \
	done
clean:
	@rm -f *.o
