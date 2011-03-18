# Experimental x86 Microkernel

Licensed under the terms of the MIT License. See the `LICENSE` file for details.

This project is an experimental microkernel built to explore operating system design and low-level x86 architecture. It is designed to run in Protected Mode and is compatible with GRUB and other Multiboot-compliant bootloaders. 

While primarily an educational exercise in systems programming, the project implements foundational kernel concepts including memory management, basic multitasking, and a Virtual File System (VFS) layer.

## Dependencies

The project was built and tested with the following toolchain:

* GNU GCC 4.7.2
* NASM 2.10.01
* GNU ld 2.22.90
* GNU Make 3.81

**Optional Emulators:**
* QEMU 1.2.0
* Bochs 2.4.6

## Build Instructions

Compile the kernel source:
```bash
make kernel
```

Generate bootable images (requires `genisoimage` for CD-ROM):
```bash
make cdrom
make floppy
```

Run the compiled image in an emulator (requires `qemu` or `bochs`):
```bash
make run-cdrom
make run-floppy
```

Build configuration and options can be adjusted in `common.mk`.

## Roadmap / Future Architecture Goals

### Core System
* Refactor memory management unit (MMU) and descriptor tables
* Process synchronization primitives (mutexes, semaphores)
* Priority-based scheduling and tickless kernel design
* ELF executable loading and relocation
* Separation of architecture-specific code from common C abstractions

### VFS and Storage
* Unified API for underlying filesystems
* Mount registry and vnode caching
* Block cache system and DMA support
* ATAPI (CD-ROM) and ATA interface drivers

### Hardware and Drivers
* Driver manager and module registration
* IRQ and DMA allocation tracking
* Independent I/O request layer
* PCI configuration and bus enumeration
* Basic TCP/IP stack (Ethernet, ARP, IP, ICMP, UDP, TCP)

### Userland
* Port Newlib (libc) and basic POSIX syscalls
* Port Bash shell and Binutils
* VESA driver and graphical interface prototyping

## Resources

Development heavily referenced the architectural references and documentation available at [OSdev.org](http://wiki.osdev.org/).