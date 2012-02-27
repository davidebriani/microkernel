#include "kernel/timer.h"
#include "kernel/arch/x86/isr.h"
#include "kernel/arch/x86/ports.h"
#include "kernel/panic.h"
#include "kernel/arch/x86/task.h"

/* This will keep track of how many ticks that the system has been running for */
uint32_t timer_ticks = 0;

static void timer_callback(registers_t *regs) {
    timer_ticks++;
    /* We're likely to be multitasking-enabled, so... */
    ASSERT(SWITCH_TASK_FREQ > 0);		/* Be sure that processes have time for living */
    if (!(timer_ticks % SWITCH_TASK_FREQ))	/* Delay */
	task_switch();
}

void init_timer() {
    /* Firstly, register our timer callback. */
    register_interrupt_handler(IRQ0, &timer_callback);
    /* Now set the timer phase */
    timer_phase(TIMER_FREQ);
}

/* This will continuously loop until the given time has been reached */
void timer_uwait(uint32_t ticks) {
    uint32_t eticks;

    eticks = timer_ticks + ticks;
    while(timer_ticks < eticks);
}

/* Set the timer phase */
void timer_phase(uint32_t hz) {
    /* The value we send to the PIT is the value to divide it's input clock
    *  (1193180 Hz) by, to get our required frequency. Important to note is
    *  that the divisor must be small enough to fit into 16-bits. */
    uint32_t divisor = 1193180 / hz;			/* Calculate our divisor */
    outb(0x43, 0x36);					/* Send our command byte 0x36 */
    outb(0x40, (uint8_t)(divisor & 0xFF));		/* Set low byte of divisor */
    outb(0x40, (uint8_t)((divisor >> 8) & 0xFF));	/* Set high byte of divisor */
}

/* System uptime in seconds */
uint32_t uptime() {
    return timer_ticks / TIMER_FREQ;
}
