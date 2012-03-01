#ifndef TIMER_H
#define TIMER_H

#include "kernel/stdint.h"

#define TIMER_FREQ		1000
#define SWITCH_TASK_FREQ	50

/* Install the timer handler && setup the system clock */
void timer_init(void);

/* Set the timer phase in hz */
void timer_phase(uint32_t frequency);

/* Loop until the given time has been reached */
void timer_uwait(uint32_t ticks);

/* Get the current system uptime in seconds */
uint32_t uptime(void);

/* A macro to wait the given time in seconds */
#define sleep(x)	timer_uwait(x*TIMER_FREQ)
/* A macro to wait the given time in mseconds */
#define usleep(x)	timer_uwait(x)

#endif
