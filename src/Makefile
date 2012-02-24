ROOTDIR := ..
INCLUDES := -I../include
include $(ROOTDIR)/common.mk

all: assemble $(OBJFILES)

clean:
	-@rm *.o
	-@cd asm && make clean
	-@cd lib && make clean
	-@cd video && make clean
	-@cd fs && make clean
	-@cd audio && make clean

assemble:
	cd asm && make
	cd lib && make
	cd video && make
	cd fs && make
	cd audio && make
