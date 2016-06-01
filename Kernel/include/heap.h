#ifndef _HEAP_H_
#define _HEAP_H_

#include <define.h>

void heap_init(void);

void * heap_alloc(unsigned int size);
void * head_zalloc(unsigned int size);

void heap_free(void * adrr);

#endif