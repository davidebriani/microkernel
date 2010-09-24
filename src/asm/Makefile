NASM=nasm
ASFLAGS=-felf

SRC=boot.s dt.s interrupt.s

all: $(SRC)
	for sfile in $(SRC); do \
	    $(NASM) $(ASFLAGS) $$sfile; \
	done
clean:
	@rm -f *.o
