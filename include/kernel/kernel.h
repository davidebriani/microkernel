#ifndef KERNEL_H
#define KERNEL_H

#include "kernel/stdint.h"
#include "kernel/video/vga.h"
#include "kernel/arch/x86/dt.h"
#include "kernel/arch/x86/paging.h"
#include "kernel/timer.h"
#include "kernel/kb.h"
#include "kernel/heap.h"
#include "kernel/arch/x86/system.h"
#include "kernel/fs/vfs.h"
#include "kernel/multiboot.h"
#include "kernel/initrd.h"
#include "kernel/panic.h"
#include "kernel/arch/x86/task.h"
#include "kernel/arch/x86/syscall.h"
#include "kernel/audio/speaker.h"
#include "kernel/shell.h"

#define DEBUG 0
#define TEST 0

#endif
