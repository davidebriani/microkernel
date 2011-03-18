ROOTDIR := ..
include $(ROOTDIR)/common.mk
CFLAGS += -I$(ROOTDIR)/include

.PHONY: all clean

all: $(OBJFILES)
	cd arch/$(ARCH) && make
	cd lib && make
	cd video && make
	cd vfs && make
	cd audio && make

clean:
	-@rm -f *.o
	-@cd arch/$(ARCH) && make clean
	-@cd lib && make clean
	-@cd video && make clean
	-@cd vfs && make clean
	-@cd audio && make clean
