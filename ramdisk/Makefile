ROOTDIR := ..
include $(ROOTDIR)/common.mk

.PHONY: all clean

all:
	cp $(ROOTDIR)/mods/*.ko root/mods/
	nm $(ROOTDIR)/$(KERNEL) | grep -f root/boot/kernel.sym > root/boot/kernel.map
	cd root/ && tar -cvf $(RAMDISK) *
	mv root/$(RAMDISK) $(ROOTDIR)/

clean:
	-@for file in $(shell find -L root -type f -name "*.ko"); do \
	    rm -f $$file; \
	done
	-@rm -f root/boot/kernel.map
