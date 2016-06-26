#ifndef _INTERRUPTS_H_
#define _INTERRUPTS_H_

#include <kdefine.h>

/**
 * Enables INTR hardware interrupts
 */
void interrupt_set(void);

/**
 * Disables INTR hardware interrupts
 */
void interrupt_clear(void);

extern void _interrupt_20(uint64_t stackF);
extern void _interrupt_21(void);
extern void _interrupt_80(void);

#endif