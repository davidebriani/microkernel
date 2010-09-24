SOURCES=string.o ports.o

CC=gcc
CFLAGS=-nostdlib -nostdinc -fno-builtin -fno-stack-protector -Wall -Wextra -pedantic -I../../include -nostartfiles -nodefaultlibs -ffreestanding -fstrength-reduce -finline-functions -fomit-frame-pointer

all: $(SOURCES)

clean:
	@rm *.o
