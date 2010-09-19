#include "system.h"
#include "ports.h"
#include "string.h"

/* This will keep track of how many ticks that the system
*  has been running for */
uint32_t timer_ticks = 0;

/* Set the timer phase */
void timer_phase(int32_t hz) {
    int32_t divisor = 1193180 / hz;	/* Calculate our divisor */
    outportb(0x43, 0x36);		/* Set our command byte 0x36 */
    outportb(0x40, divisor & 0xFF);	/* Set low byte of divisor */
    outportb(0x40, (divisor >> 8) & 0xFF);	/* Set high byte of divisor */
}

/* Handles the timer. In this case, it's very simple: We
*  increment the 'timer_ticks' variable every time the
*  timer fires. By default, the timer fires 18.222 times
*  per second. Why 18.222Hz? Some engineer at IBM must've
*  been smoking something funky */
static void timer_handler(struct regs *r) {
    /* Increment our 'tick count' */
    timer_ticks++;

    /* Every 18 clocks (approximately 1 second), we will
    *  display a message on the screen */
    if (timer_ticks % 18 == 0) {
        puts("One second has passed\n");
    }
}

/* This will continuously loop until the given time has
*  been reached */
void timer_wait(int32_t ticks) {
    unsigned long eticks;

    eticks = timer_ticks + ticks;
    while((unsigned long)timer_ticks < eticks);
}

/* Sets up the system clock by installing the timer handler
*  into IRQ0 */
void init_timer() {
    /* Installs 'timer_handler' to IRQ0 */
    irq_install_handler(IRQ0, &timer_handler);
}
