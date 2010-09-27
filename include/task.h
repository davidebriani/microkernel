#ifndef TASK_H
#define TASK_H

#include "stdint.h"
#include "paging.h"

/* This structure defines a 'task' - a process */
typedef struct task
{
    int32_t id;				/* Process ID */
    uint32_t esp, ebp;			/* Stack and base pointers */
    uint32_t eip;			/* Instruction pointer */
    page_directory_t *page_directory;	/* Page directory */
    struct task *next;			/* The next task in a linked list */
} task_t;

/* Initialises the tasking system */
void init_tasking();

/* Called by the timer hook, this changes the running process */
void task_switch();

/* Forks the current process, spawning a new one with a different memory space */
int32_t fork();

/* Causes the current process' stack to be forcibly moved to a new location */
void move_stack(void *new_stack_start, uint32_t size);

/* Returns the pid of the current process */
int32_t getpid();

#endif
