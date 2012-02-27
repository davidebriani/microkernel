ROOTDIR := ..
include $(ROOTDIR)/common.mk
CFLAGS += -I$(ROOTDIR)/include

.PHONY: all clean

all: $(OBJFILES)
	cd arch/$(ARCH) && make

clean:
	-@cd arch/$(ARCH) && make clean
