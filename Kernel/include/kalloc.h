#ifndef _KALLOC_H_
#define _KALLOC_H_

#include <kdefine.h>

void * kmalloc(unsigned int size);
void * kcalloc(unsigned int size);

void kfree(void * adrr);

#endif
