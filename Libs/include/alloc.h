#ifndef _ALLOC_H_
#define _ALLOC_H_

void * alloc(unsigned int count, unsigned int size);
void * salloc(unsigned int size);

void * zalloc(unsigned int count, unsigned int size);
void * zsalloc(unsigned int size);

// void * realloc(void * addr, unsigned int count, unsigned int size);
// void * srealloc(void * addr, unsigned int size);

void free(void * addr);

#endif
