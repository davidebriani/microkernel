#include <kernel/stdint.h>
#include <kernel/audio/speaker.h>
#include <kernel/arch/x86/io.h>
#include <kernel/timer.h>

static void speaker_phase(uint32_t freq)
{
    uint32_t div;

    /* set the PIT 2 to the desired frequency */
    div = 1193180 / freq;
    io_outb(0x43, 0xb6);
    io_outb(0x42, (uint8_t) (div));
    io_outb(0x42, (uint8_t) (div >> 8));
}

static void speaker_on(void)
{
    uint8_t tmp;
    /* play the sound using the PC speaker */
    tmp = io_inb(0x61);
    if (tmp != (tmp | 3))
	io_outb(0x61, (tmp | 3));
}

static void speaker_off(void)
{
    uint8_t tmp = (io_inb(0x61) & 0xFC);
    io_outb(0x61, tmp);
}

void ubeep(uint32_t freq, uint32_t time)
{
    speaker_phase(freq);
    speaker_on();
    usleep(time);
    speaker_off();
    speaker_phase(TIMER_FREQ);
}
