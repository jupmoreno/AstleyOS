#include <alloc.h>
#include <memory.h>

extern void * sysalloc_new(unsigned int size);
extern void sysalloc_free(void * addr);
// extern void sysalloc_realloc(void * addr, unsigned int size);

void * alloc(unsigned int count, unsigned int size) {
	return salloc(count * size);
}

void * salloc(unsigned int size) {
	if(size == 0) {
		return NULL;
	}

	return sysalloc_new(size);
}

void * zalloc(unsigned int count, unsigned int size) {
	return zsalloc(count * size);
}

void * zsalloc(unsigned int size) {
	void * addr;

	if(size == 0) {
		return NULL;
	}

	addr = sysalloc_new(size);
	memset(addr, 0, size);

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
