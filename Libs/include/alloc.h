#ifndef _ALLOC_H_
#define _ALLOC_H_

#include <define.h>

void * alloc(unsigned int count, unsigned int bytes);
void * malloc(unsigned int bytes);

void * calloc(unsigned int count, unsigned int bytes);

// void * realloc(void * addr, unsigned int count, unsigned int size);
// void * srealloc(void * addr, unsigned int size);

void free(void * addr);

#endif
