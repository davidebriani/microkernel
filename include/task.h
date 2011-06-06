#ifndef TASK_H
#define TASK_H

#include "stdint.h"
#include "paging.h"

#define KERNEL_STACK_SIZE    2048    /* Use a 2kb kernel stack */

/* This structure defines a 'task' - a process */
typedef struct task
{
    int32_t id;				/* Process ID */
    uint32_t esp, ebp;			/* Stack and base pointers */
    uint32_t eip;			/* Instruction pointer */
    page_directory_t *page_directory;	/* Page directory */
    uint32_t kernel_stack;		/* Kernel stack location */
    struct task *next;			/* The next task in a linked list */
} task_t;

/* Initialises the tasking system */
void init_tasking(void);

/* Called by the timer hook, this changes the running process */
void task_switch(void);

/* Forks the current process, spawning a new one with a different memory space */
int32_t fork(void);

/* Causes the current process' stack to be forcibly moved to a new location */
void move_stack(void *new_stack_start, uint32_t size);

/* Returns the pid of the current process */
int32_t getpid(void);

/* Temp forks and executes func(), returns child's pid */
int32_t task_init(void func(void));

/* Attempts to kill a process. Returns 0 on success, -1 on failure */
int32_t task_kill(int32_t pid);

/* Simulate a return from an exception that began in usermode */
void init_usermode(void);

#endif
