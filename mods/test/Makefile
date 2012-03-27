ROOTDIR := ../..
include $(ROOTDIR)/common.mk
CFLAGS += -I$(ROOTDIR)/include
LDFLAGS += -r

.PHONY: all clean

all: $(OBJFILES)
	@${LD} ${LDFLAGS} $(OBJFILES) -o test.ko

clean:
	-@rm -f *.o *.ko
