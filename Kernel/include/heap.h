#ifndef _HEAP_H_
#define _HEAP_H_

#include <kdefine.h>

void heap_init(void);

void * heap_pages(unsigned int pages);
void heap_free(void * adrr);

#endif
