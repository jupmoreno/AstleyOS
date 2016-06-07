#include <heap.h>
#include <numbers.h>
#include <memory.h>

/**
 * Buddy implementation from: https://github.com/cloudwu/buddy
 *
 * MORE INFO:
 * http://bitsquid.blogspot.com.ar/2015/08/allocation-adventures-3-buddy-allocator.html
 */

// To change heap size, change:
#define HEAP_ALLOC_SIZE		MEGABYTES(512)
// Remember to change kernel's/userland's initial memory location
// ----------------------------

// To change heap location, change:
#define HEAP_MEMORY_BASE	0x100000
// Remember to change kernel's/userland's initial memory location
// ----------------------------

#define HEAP_MEMORY_SIZE	(HEAP_ALLOC_SIZE + sizeof(allocator_st))

#define HEAP_STRUCT_BASE	(HEAP_MEMORY_BASE + HEAP_ALLOC_SIZE)
#define HEAP_STRUCT_TREE	(HEAP_ALLOC_SIZE / _MEMORY_PAGE_SIZE) * 2 - 1
#define HEAP_STRUCT_LVLS	17 // (HEAP_ALLOC_SIZE / _MEMORY_PAGE_SIZE) = 2^lvls

#define BUDDY_UNUSED		0
#define BUDDY_USED			1
#define BUDDY_SPLITTED		2
#define BUDDY_FULL			3

typedef struct {
	unsigned int size;
	int levels;
	uint8_t tree[HEAP_STRUCT_TREE];
} allocator_st;

static int buddy_alloc(unsigned int size);
static void buddy_free(unsigned int offset);
static int buddy_offset(int index, int level);
static void buddy_markParent(int index);
static void buddy_combine(int index);

static allocator_st * allocator; // = {(1 << 17), 17, {BUDDY_UNUSED}}; // TODO: Calcular el 17 & Inicializar en 0 tree

void heap_init(void) {
	// memset(HEAP_MEMORY_BASE, 0, HEAP_ALLOC_SIZE);
	allocator = (allocator_st *) HEAP_STRUCT_BASE;

	allocator->size = HEAP_ALLOC_SIZE / _MEMORY_PAGE_SIZE;
	allocator->levels = HEAP_STRUCT_LVLS;
	memset(allocator->tree, BUDDY_UNUSED, HEAP_STRUCT_TREE);
}

void * heap_pages(unsigned int pages) {
	int offset;

	if(!pages) {
		#ifdef _DEGUB_ENABLED
		log("<HEAP> Invalid pages: (%d)\n", pages);
		#endif
		return NULL;
	}

	offset = buddy_alloc(pages * _MEMORY_PAGE_SIZE);
	if(offset == -1) {
		#ifdef _DEGUB_ENABLED
		log("<HEAP> Couldn't alloc: (%d)\n", pages);
		#endif
		return NULL;
	}

	#ifdef _DEGUB_ENABLED
	log("<HEAP> Alloc: (%d) -> %h\n", pages, HEAP_MEMORY_BASE + offset);
	#endif

	return (void *) ((intptr_t) (HEAP_MEMORY_BASE + offset));
}

void heap_free(void * addr) {
	buddy_free(((intptr_t) addr - HEAP_MEMORY_BASE) / _MEMORY_PAGE_SIZE);
}

static int buddy_alloc(unsigned int size) {
	int index = 0, level = 0;
	unsigned int length = allocator->size;

	if(size == 0) {
		return -1;
	}

	size = numnextPow2(size);

	if(size > length) {
		return -1;
	}

	while(index >= 0) {
		if(size == length) {
			if(allocator->tree[index] == BUDDY_UNUSED) {
				allocator->tree[index] = BUDDY_USED;
				buddy_markParent(index);
				return buddy_offset(index, level);
			}
		} else {
			// size < length
			switch(allocator->tree[index]) {
				case BUDDY_USED:
				case BUDDY_FULL:
					break;

				case BUDDY_UNUSED:
					// split first
					allocator->tree[index] = BUDDY_SPLITTED;
					allocator->tree[index * 2 + 1] = BUDDY_UNUSED;
					allocator->tree[index * 2 + 2] = BUDDY_UNUSED;

				default:
					index = index * 2 + 1;
					length /= 2;
					level++;
					continue;
			}
		}

		if(index & 1) {
			++index;
			continue;
		}

		for(;;) {
			level--;
			length *= 2;
			index = (index + 1) / 2 - 1;

			if(index < 0) {
				return -1;
			}

			if(index & 1) {
				++index;
				break;
			}
		}
	}

	return -1;
}

static void buddy_free(unsigned int offset) {
	int index = 0;
	unsigned int length = allocator->size, left = 0;

	if(offset >= length) {
		// TODO: This should never happend
		return;
	}

	for(;;) {
		switch(allocator->tree[index]) {
			case BUDDY_USED:
				if(offset != left) {
					// TODO: This should never happend
					return;
				}
				buddy_combine(index);
				return;

			case BUDDY_UNUSED:
				// TODO: This should never happend
				return;

			default:
				length /= 2;
				if(offset < left + length) {
					index = index * 2 + 1;
				} else {
					left += length;
					index = index * 2 + 2;
				}
				break;
		}
	}
}

static int buddy_offset(int index, int level) {
	return ((index + 1) - (1 << level)) << (allocator->levels - level);
}

static void buddy_markParent(int index) {
	int buddy;

	for(;;) {
		buddy = index - 1 + (index & 1) * 2;

		if(buddy > 0 && (allocator->tree[buddy] == BUDDY_USED || allocator->tree[buddy] == BUDDY_FULL)) {
			index = (index + 1) / 2 - 1;
			allocator->tree[index] = BUDDY_FULL;
		} else {
			return;
		}
	}
}

static void buddy_combine(int index) {
	int buddy;

	for(;;) {
		buddy = index - 1 + (index & 1) * 2;

		if(buddy < 0 || allocator->tree[buddy] != BUDDY_UNUSED) {
			allocator->tree[index] = BUDDY_UNUSED;

			while(((index = (index + 1) / 2 - 1) >= 0) &&  allocator->tree[index] == BUDDY_FULL) {
				allocator->tree[index] = BUDDY_SPLITTED;
			}

			return;
		}

		index = (index + 1) / 2 - 1;
	}
}
