#ifndef SPEAKER_H
#define SPEAKER_H

#include "kernel/stdint.h"

/* Make the speaker beep at the given 'freq'uency for 'time' secs */
void ubeep(uint32_t freq, uint32_t time);

#define beep(f,t) ubeep(f,t*1000)

#endif
