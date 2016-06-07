#ifndef _PIT_H_
#define _PIT_H_

#include <kdefine.h>

void pit_init(void);
// void pit_set(unsigned int miliseconds);
void pit_wait(double seconds);

#endif
