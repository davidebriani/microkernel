#include "task.h"
#include "paging.h"
#include "heap.h"
#include "string.h"
#include "timer.h"

/* The currently running task */
volatile task_t *current_task;

/* The start of the task linked list */
volatile task_t *ready_queue;

/* Some externs are needed to access members in paging.c... */
extern page_directory_t *kernel_directory;
extern page_directory_t *current_directory;
extern void alloc_frame(page_t*, int32_t, int32_t);
extern uint32_t initial_esp;
extern uint32_t read_eip();

/* The next available process ID */
uint32_t next_pid = 1;

void init_tasking()
{
    /* Rather important stuff happening, no interrupts please! */
    asm volatile("cli");

    /* Relocate the stack so we know where it is */
    move_stack((void *) 0xE0000000, 0x2000);

    /* Initialise the first task (kernel task) */
    current_task = ready_queue = (task_t*) kmalloc(sizeof(task_t));
    current_task->id = next_pid++;
    current_task->esp = current_task->ebp = 0;
    current_task->eip = 0;
    current_task->page_directory = current_directory;
    current_task->next = 0;

    /* Reenable interrupts */
    asm volatile("sti");
}

void move_stack(void *new_stack_start, uint32_t size)
{
    uint32_t i;
    /* Allocate some space for the new stack */
    for( i = (uint32_t) new_stack_start; i >= ((uint32_t) new_stack_start-size); i -= 0x1000)
    {
	/* General-purpose stack is in user-mode */
	alloc_frame( get_page(i, 1, current_directory), 0 /* User mode */, 1 /* Is writable */ );
    }

    /* Flush the TLB by reading and writing the page directory address again */
    uint32_t pd_addr;

    asm volatile("mov %%cr3, %0" : "=r" (pd_addr));
    asm volatile("mov %0, %%cr3" : : "r" (pd_addr));

    /* Old ESP and EBP, read from registers */
    uint32_t old_stack_pointer; asm volatile("mov %%esp, %0" : "=r" (old_stack_pointer));
    uint32_t old_base_pointer;  asm volatile("mov %%ebp, %0" : "=r" (old_base_pointer));

    /* Offset to add to old stack addresses to get a new stack address */
    uint32_t offset = (uint32_t) new_stack_start - initial_esp;

    /* New ESP and EBP */
    uint32_t new_stack_pointer = old_stack_pointer + offset;
    uint32_t new_base_pointer  = old_base_pointer  + offset;

    /* Copy the stack */
    memcpy((void *)new_stack_pointer, (void *)old_stack_pointer, initial_esp-old_stack_pointer);

    /* Backtrace through the original stack, copying new values into the new stack */
    for(i = (uint32_t)new_stack_start; i > (uint32_t)new_stack_start-size; i -= 4)
    {
	uint32_t tmp = * (uint32_t *) i;
	/* If the value of tmp is inside the range of the old stack, assume it is a base pointer
	*  and remap it. This will unfortunately remap ANY value in this range, whether they are
	*  base pointers or not */
	if (( old_stack_pointer < tmp) && (tmp < initial_esp))
	{
	    tmp = tmp + offset;
	    uint32_t *tmp2 = (uint32_t *) i;
	    *tmp2 = tmp;
	}
    }

    /* Change stacks */
    asm volatile("mov %0, %%esp" : : "r" (new_stack_pointer));
    asm volatile("mov %0, %%ebp" : : "r" (new_base_pointer));
}

void switch_task()
{
    /* If we haven't initialised tasking yet, just return */
    if (!current_task)
	return;

    /* Read esp, ebp now for saving later on */
    uint32_t esp, ebp, eip;
    asm volatile("mov %%esp, %0" : "=r"(esp));
    asm volatile("mov %%ebp, %0" : "=r"(ebp));

    /* Read the instruction pointer. We do some cunning logic here:
    *  One of two things could have happened when this function exits -
    *    (a) We called the function and it returned the EIP as requested.
    *    (b) We have just switched tasks, and because the saved EIP is essentially
    *        the instruction after read_eip(), it will seem as if read_eip has just
    *        returned.
    *  In the second case we need to return immediately. To detect it we put a dummy
    *  value in EAX further down at the end of this function. As C returns values in EAX,
    *  it will look like the return value is this dummy value! (0x12345) */
    eip = read_eip();

    /* Have we just switched tasks? */
    if (eip == 0x12345)
	return;

    /* No, we didn't switch tasks. Let's save some register values and switch */
    current_task->eip = eip;
    current_task->esp = esp;
    current_task->ebp = ebp;

    /* Get the next task to run */
    current_task = current_task->next;
    /* If we fell off the end of the linked list start again at the beginning */
    if (!current_task) current_task = ready_queue;

    eip = current_task->eip;
    esp = current_task->esp;
    ebp = current_task->ebp;

    /* Make sure the memory manager knows we've changed page directory */
    current_directory = current_task->page_directory;
    /* Here we:
    *  * Stop interrupts so we don't get interrupted.
    *  * Temporarily puts the new EIP location in ECX.
    *  * Loads the stack and base pointers from the new task struct.
    *  * Changes page directory to the physical address (physicalAddr) of the new directory.
    *  * Puts a dummy value (0x12345) in EAX so that above we can recognise that we've just
    *    switched task.
    *  * Restarts interrupts. The STI instruction has a delay - it doesn't take effect until after
    *    the next instruction.
    *  * Jumps to the location in ECX (remember we put the new EIP in there). */
    asm volatile("		\
	cli;			\
	mov %0, %%ecx;		\
	mov %1, %%esp;		\
	mov %2, %%ebp;		\
	mov %3, %%cr3;		\
	mov $0x12345, %%eax;	\
	sti;			\
	jmp *%%ecx		" : : "r"(eip), "r"(esp), "r"(ebp), "r"(current_directory->physicalAddr));
}

int32_t fork()
{
    /* We are modifying kernel structures, and so cannot */
    asm volatile("cli");

    /* Take a pointer to this process' task struct for later reference */
    task_t *parent_task = (task_t *) current_task;

    /* Clone the address space */
    page_directory_t *directory = clone_directory(current_directory);

    /* Create a new process */
    task_t *new_task = (task_t *) kmalloc(sizeof(task_t));

    new_task->id = next_pid++;
    new_task->esp = new_task->ebp = 0;
    new_task->eip = 0;
    new_task->page_directory = directory;
    new_task->next = 0;

    /* Add it to the end of the ready queue */
    task_t *tmp_task = (task_t *) ready_queue;
    while (tmp_task->next)
	tmp_task = tmp_task->next;
    tmp_task->next = new_task;

    /* This will be the entry point for the new process */
    uint32_t eip = read_eip();


    /* We could be the parent or the child here - check */
    if (current_task == parent_task)
    {
	/* We are the parent, so set up the esp/ebp/eip for our child */
	uint32_t esp; asm volatile("mov %%esp, %0" : "=r"(esp));
	uint32_t ebp; asm volatile("mov %%ebp, %0" : "=r"(ebp));
	new_task->esp = esp;
	new_task->ebp = ebp;
	new_task->eip = eip;
	asm volatile("sti");

	return new_task->id;
    }
    else
    {
	/* We are the child */
	return 0;
    }
}

int32_t getpid()
{
    return current_task->id;
}

void init_proc(void func())
{
    int32_t ret = fork();
    int32_t pid = getpid();

    /* If we are the child */
    if (!ret)
    {
	/* Execute the requested function */
	func();
	/* Kill the current (child) process. On failure, freeze it */
	if(!kill_proc(pid)) for(;;);
    }
    return;
}

int32_t kill_proc(int32_t pid)
{
    task_t *tmp_task = (task_t *) ready_queue;
    task_t *par_task = tmp_task;

    /* Find the process in the proc. list */
    while (tmp_task->id != pid) {
        par_task = tmp_task;
        tmp_task = tmp_task->next;
    }

    /* Can we delete it? */
    if (tmp_task != par_task) {
        par_task->next = tmp_task->next;
        /* Wait 1 second so that we have time to switch
        * to another process before this one returns: we
        * are killing the child! */
        timer_wait(1);
	return 0;
    } else {
        return -1;
    }
}
