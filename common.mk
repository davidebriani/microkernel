ARCH := x86
include $(ROOTDIR)/$(ARCH).mk

SRCFILES := $(shell find -L . -maxdepth 1 -type f -name "*.c")
OBJFILES := $(patsubst %.c,%.o,$(SRCFILES))
CC := gcc
WARNINGS := -Wall -Wextra -ansi -pedantic -Wno-unused-parameter -Wshadow -Wpointer-arith -Wcast-align -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations -Wredundant-decls -Wnested-externs -Winline -Wno-long-long -Wuninitialized -Wstrict-prototypes
CFLAGS := -static -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -ffreestanding -fstrength-reduce -finline-functions -std=c99 $(WARNINGS)

ASM := nasm
ASMFLAGS := -felf

AR := ar
ARFLAGS := rvs

LD := ld

VM := qemu
