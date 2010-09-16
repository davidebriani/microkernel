GCC_FLAGS=-Wall -Wextra -nostdlib -nostdinc -nostartfiles -nodefaultlibs -fno-builtin -fno-stack-protector -ffreestanding -fstrength-reduce -fomit-frame-pointer -finline-functions -O2 -I../include

GCC=gcc

SRC=string.c ports.c

all: $(SRC)
	$(GCC) $(GCC_FLAGS) -c $(SRC)

clean:
	@rm -f *.o
