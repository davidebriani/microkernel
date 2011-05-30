SOURCES := textmode.o

CC := gcc
WARNINGS := -Wall -Wextra -ansi -pedantic -Wno-unused-parameter -Wshadow -Wpointer-arith -Wcast-align -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations -Wredundant-decls -Wnested-externs -Winline -Wno-long-long -Wuninitialized -Wstrict-prototypes
CFLAGS := -nostdlib -nostdinc -fno-builtin -fno-stack-protector -Wall -Wextra -pedantic -I../../include -nostartfiles -nodefaultlibs -ffreestanding -fstrength-reduce -finline-functions -fomit-frame-pointer $(WARNINGS)

all: $(SOURCES)

clean:
	-@rm *.o
