#ifndef _LIB_MEMORY_H_
#define _LIB_MEMORY_H_

#include <stdint.h>
#include <stddef.h>

void * memset(void * destination, int32_t c, uint64_t length);

void * memcpy(void * destination, const void * source, uint64_t length);

// TODO: Check
int memcmp(const void * s1,	const void * s2, size_t n);

#endif