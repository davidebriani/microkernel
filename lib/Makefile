ROOTDIR := ..
include $(ROOTDIR)/common.mk
CFLAGS += -I$(ROOTDIR)/include

.PHONY: all clean

all: $(OBJFILES)
	cd arch/$(ARCH) && make

clean:
	-@rm -f *.o
	-@cd arch/$(ARCH) && make clean
