ROOTDIR := ..
include $(ROOTDIR)/common.mk

.PHONY: all clean

all:
	-@if [ ! -d root/mods/ ]; then mkdir root/mods/; fi
	for file in $(shell find -L $(ROOTDIR)/mods -type f -name "*.ko"); do \
	    cp $$file root/mods/; \
	done
	nm $(ROOTDIR)/$(KERNEL) | grep -f root/boot/kernel.sym > root/boot/kernel.map
	cd root/ && tar -cvf $(RAMDISK) *
	mv root/$(RAMDISK) $(ROOTDIR)/

clean:
	-@rm -f root/mods/*.ko root/boot/kernel.map
