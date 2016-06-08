#include <alloc.h>
#include <memory.h>

extern void * sysalloc_new(unsigned int size);
extern void sysalloc_free(void * addr);
// extern void sysalloc_realloc(void * addr, unsigned int size);

void * alloc(unsigned int count, unsigned int bytes) {
	return malloc(count * bytes);
}

void * malloc(unsigned int bytes) {
	if(bytes == 0) {
		return NULL;
	}

	return sysalloc_new(1 + ((bytes - 1) / _MEMORY_PAGE_SIZE));
}

void * calloc(unsigned int count, unsigned int bytes) {
	void * addr;

	addr = malloc(count * bytes);
	if(addr == NULL) {
		return NULL;
	}
	memset(addr, 0, count * bytes);

	return addr;
}

// void * realloc(void * addr, unsigned int count, unsigned int size) {
// 	;
// }

// void * srealloc(void * addr, unsigned int size) {
// 	;
// }

void free(void * addr) {
	sysalloc_free(addr);
}
