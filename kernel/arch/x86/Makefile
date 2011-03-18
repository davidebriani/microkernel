ROOTDIR := ../../..
include $(ROOTDIR)/common.mk
CFLAGS += -I$(ROOTDIR)/include
ASMFILES := $(shell ls *.s)
ASMFLAGS += -w-number-overflow

.PHONY: all clean

all: $(ASMFILES) $(OBJFILES)
	for sfile in $(ASMFILES); do \
	    $(ASM) $(ASMFLAGS) $$sfile; \
	done
clean:
	-@rm -f *.o
