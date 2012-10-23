## Yet another (micro?)kernel

Licensed under the terms of the MIT Licence: see LICENSE file for details.

This is not a true, functional kernel or OS. This is a toy to play with: I was trying to learn something interesting about kernels and how they work and I came up with this; you'll forgive me for being such a newbie...
The... thing is currently designed for x86 cpu only, gets started by GRUB (or any other multiboot compliant bootloader), and works in Protected Mode. It's in a very early state of development and, of course, it is absolutely not useful.



## Dependencies (tools it is known to work with)

    GNU gcc	4.7.2-2
    Nasm	2.10.01
    GNU ld	2.22.90
    GNU Make	3.81

    Optional:
    Qemu	1.2.0
    Bochs	2.4.6



## Build Instructions

    make kernel		(compile and glue together source files)

    make cdrom		(create a cdrom bootable image)
    make floppy		(create a floppy bootable image; will need 'genisoimage')

    make run-cdrom	(boot and test the cdrom image; will need 'qemu' or 'bochs')
    make run-floppy	(boot and test the floppy image; will need 'qemu' or 'bochs')

    vim common.mk	(edit configuration and options; will need 'vim')



## TODO

### General - Urgent

    * REWRITE kernel code: mmu, multitasking and descr. tables code
    * cleanups
    * vfs functions & file editing improvements
    * improve syscall system
    * separate x86 code from C common code
    * how to implement a decent stdio?

### VFS Layer

    * think about a good design
    * API for underlying filesystems
    * mounting, unmounting, mount lists
    * caches (vnode, etc.)
    * task-related things

### Multitasking

    * ELF executable & relocatable loading
    * process synchronization (mutexes, semaphores)
    * priority based scheduling (timeslices?)
    * tickless kernel?

### Newlib - libc port

    * Basic syscalls
    * UNIX syscalls
    * POSIX syscalls

### Drivers

    * driver manager?
    * drivers & modules registration
    * IRQ registration
    * DMA registration
    * I/O request layer (independent of VFS)
    * I/O request queue

### Disk

    * CD-ROM drive support (ATAPI devices)
    * ATA
    * Floppy
    * Block cache system
    * Disk DMA support

### PCI bus

    * PCI configuration
    * PCI bus layer

### Networking

    * TCP/IP stack (Ethernet, ARP, IP, ICMP, UDP, TCP)
    * Card drivers

### Other

    * port binutils, gcc
    * port bash shell
    * vesa driver, graphical interface



## Documents/links

There is no documentation for this project yet.

If you're trying to understand the code or thinking about developing your own OS, you may find very useful documentation/tutorials at [OSdev.org](http://wiki.osdev.org/Main_Page).
