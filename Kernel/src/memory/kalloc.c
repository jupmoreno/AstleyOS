#include <kalloc.h>
#include <heap.h>
#include <memory.h>

void * kmalloc(unsigned int size) {
	void * addr;

	addr = heap_pages(size / _MEMORY_PAGE_SIZE + 1);
	if(addr == NULL) {
		log("ERRORRRRR\n");
		return NULL;
	}

	return addr;
}

void * kcalloc(unsigned int size) {
	void * addr;

	addr = kmalloc(size);
	if(addr == NULL) {
		return NULL;
	}
	memset(addr, 0, size);

	return addr;
}

void kfree(void * addr) {
	heap_free(addr);
}
