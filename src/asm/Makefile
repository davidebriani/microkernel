NASM := nasm
ASFLAGS := -felf

SRC := $(shell ls *.s)

all: $(SRC)
	for sfile in $(SRC); do \
	    $(NASM) $(ASFLAGS) $$sfile; \
	done
clean:
	-@rm -f *.o
