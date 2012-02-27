ROOTDIR := ../../..
include $(ROOTDIR)/common.mk
CFLAGS += -I$(ROOTDIR)/include

.PHONY: all clean

all: $(OBJFILES)
	@#${ASM} ${ASMFLAGS} crt0.s -o crt0.o

clean:
	-@rm -f *.o

