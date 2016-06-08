#include <kalloc.h>
#include <heap.h>
#include <memory.h>

void * kmalloc(unsigned int bytes) {
	return heap_pages(1 + ((bytes - 1) / _MEMORY_PAGE_SIZE));
}

void * kcalloc(unsigned int bytes) {
	void * addr;

	addr = kmalloc(bytes);
	if(addr == NULL) {
		return NULL;
	}
	memset(addr, 0, bytes);

	return addr;
}

void kfree(void * addr) {
	heap_free(addr);
}
