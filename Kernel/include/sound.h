#ifndef _SOUND_H_
#define _SOUND_H_

#include <kdefine.h>

uint64_t sound_beep(unsigned int frequency, double time);

static void sound_play(unsigned int frequency);
static void sound_stop(void);
#endif