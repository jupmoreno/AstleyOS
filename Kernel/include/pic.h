#ifndef _PIC_H_
#define _PIC_H_

#include <define.h>

#define _PIC_ERROR_IRQ_INVALID -1

#define _PIC_MASK_ALL 0xFF

void pic_init();

/**
 * Applies the given mask to the PIC to enable/disable interrupts
 * @param i - the mask to be applied to the PIC    
 */
void pic_mask(uint8_t mask);
void pic_mask_all();

int pic_irq_mask(uint8_t irq);
int pic_irq_clear(uint8_t irq);
int pic_irq_eoi(uint8_t irq);

/**
 * Returns the combined value of the cascaded PICs irq request register
 */
uint16_t pic_irr();

/**
 * Returns the combined value of the cascaded PICs in-service register
 */
uint16_t pic_isr();

#endif
