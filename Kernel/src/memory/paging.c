#include <paging.h>
#include <define.h>
#include <log.h>

// http://www.tutorialspoint.com/cprogramming/c_bit_fields.htm
typedef struct {
	uint64_t
		pe			: 1,	// Protection Enable
		mp			: 1,	// Monitor Coprocessor
		em			: 1,	// Emulation
		ts			: 1,	// Task Switched
		et			: 1,	// Extension Type
		ne			: 1,	// Numeric Error
		zero_1		: 10, 	// Reserved
		wp			: 1,	// Write Protect
		zero_2		: 1, 	// Reserved
		am			: 1,	// Alignment Mask
		zero_3		: 10, 	// Reserved
		nw			: 1,	// Not Write-through
		cd			: 1,	// Cache Disable
		pg			: 1,	// Paging
		zero_4		: 32; 	// Bits 63:32 of CR0 are reserved and must be written with zeros
} cr0_t;

// typedef struct {
// 	uint64_t
// 		zero_1		: 3,	// Reserved
// 		pwt			: 1,	// Page-level Write-Through
// 		pcd			: 1,	// Page-level Cache Disable
// 		zero_2		: 7,	// Reserved
// 		base_1		: 28,
// 		// ???		: 8,
// 		zero_3		: 12, 	// Bits 51:40 of CR3 are reserved and must be 0
// 		base_2		: 12;	// ???
// } cr3_t; // Normal CR3

typedef struct {
	uint64_t
		zero_1		: 5,	// Ignored
		base		: 27,	// Physical address of the 32-Byte aligned page-directory-pointer table
		zero_2		: 32;	// Ignored
} cr3_t; // PAE CR3

typedef struct {
	uint64_t
		p			: 1,	// Present; must be 1 to reference a page directory
		zero_1		: 2,	// Reserved (must be 0)
		pwt			: 1,	// Page-level write-through
		pcd			: 1,	// Page-level cache disable
		zero_2		: 4,	// Reserved (must be 0)
		zero_3		: 3,	// Ignored
		base		: 40,	// Physical address of 4-KByte aligned page directory
		zero_4		: 12;	// Reserved (must be 0)
} pdpte_st;

typedef struct {
	uint64_t
		p			: 1,	// Present; must be 1 to reference a page table
		rw			: 1,	// Read/write
		us			: 1,	// User/supervisor
		pwt			: 1,	// Page-level write-through
		pcd			: 1,	// Page-level cache disable
		a			: 1,	// Accessed
		zero_1		: 1,	// Ignored
		ps			: 1,	// Page size; must be 0 (otherwise, this entry maps a 2-MByte page)
		zero_2		: 4,	// Ignored
		base		: 40,	// Physical address of 4-KByte aligned page table
		zero_3		: 11,	// Reserved (must be 0)
		xd			: 1;	// Reserved (must be 0)
} pdte_st;

typedef struct {
	uint64_t
		p			: 1,	// Present; must be 1 to map a 4-KByte page
		rw			: 1,	// Read/write
		us			: 1,	// User/supervisor
		pwt			: 1,	// Page-level write-through
		pcd			: 1,	// Page-level cache disable
		a			: 1,	// Accessed
		d			: 1,	// Dirty
		pat			: 1,	// If PAT supported, indirectly determines mem. type used to access the page
		g			: 1,	// Global
		zero_1		: 3,	// Ignored
		base		: 40,	// Physical address of 4-KByte aligned page table
		zero_2		: 11,	// Reserved (must be 0)
		xd			: 1;	// Reserved (must be 0)
} pte_st;

typedef struct {
	uint32_t
		pointer 	: 2,	// Directory Pointer
		directory	: 9,	// Page Directory
		table		: 9,	// Page table
		offset		: 12;
} address_st;

extern cr0_t _cr0_read(void);
extern void _cr0_write(cr0_t cr0);
extern cr3_t _cr3_read(void);
extern void _cr3_write(cr3_t cr3);

static void paging_enable(void);

#define PDPT_SIZE	4
static pdpte_st directory_pointers[PDPT_SIZE];
#define PDT_SIZE	512
static pdte_st directory_1[PDT_SIZE];
#define PT_SIZE		512
static pte_st table_1[PT_SIZE];

void paging_init(void) {
	log("<MEMORY> CR0: %h\n", _cr0_read());
	log("<MEMORY> CR3: %h\n", _cr3_read());
	// paging_identify();
	// paging_enable();
}

static void pdpt_init(pdpte_st * table[PDPT_SIZE]) {
	int i;
	pdpte_st * entry;

	entry = table[0];
	entry->p = 1;
	entry->zero_1 = 0;
	entry->pwt = 0;
	entry->pcd = 1;
	entry->zero_2 = 0;
	entry->zero_3 = 0;
	entry->base = (uint64_t) &directory_1;
	entry->zero_4 = 0;

	for(i = 1; i < PDPT_SIZE; i++) {
		entry = table[i];
		entry->p = 0;
	}
}

static void pdt_init(pdte_st * table[PDT_SIZE]) {
	int i;
	pdte_st * entry;

	entry = table[0];
	entry->p = 1;
	entry->rw = 1;
	entry->us = 1;
	entry->pwt = 0;
	entry->pcd = 1;
	entry->a = 1;
	entry->zero_1 = 0;
	entry->ps = 0;
	entry->zero_2 = 0;
	entry->base = (uint64_t) &table_1;
	entry->zero_3 = 0;
	entry->xd = 0;

	for(i = 1; i < PDT_SIZE; i++) {
		entry = table[i];
		entry->p = 0;
	}
}

static void pt_init(pte_st * table[PT_SIZE]) {
	int i;
	pte_st * entry;

	entry = table[0];
	entry->p = 1;
	entry->rw = 1;
	entry->us = 1;
	entry->pwt = 0;
	entry->pcd = 1;
	entry->a = 1;
	entry->d = 0;
	entry->pat = 0;
	entry->g = 0;
	entry->zero_1 = 0;
	entry->base = (uint64_t) &table_1; // TODO: Change
	entry->zero_2 = 0;
	entry->xd = 0;

	for(i = 1; i < PT_SIZE; i++) {
		entry = table[i];
		entry->p = 0;
	}
}

static void paging_enable(void) {
	// _cr0_write((uint64_t) page_directory);
	// _cr3_write((uint64_t) (_cr3_read() | 0x80000000)); // Set the paging bit in CR0 to 1
}

// uint64_t directory[1024] __attribute__((aligned(4096)));
// uint64_t first_table[1024] __attribute__((aligned(4096)));

// void paging_init() {
// 	unsigned int i;

// 	for(i = 0; i < 1024; i++) {
// 		// This sets the following flags to the pages:
// 		//   Supervisor: Only kernel-mode can access them
// 		//   Write Enabled: It can be both read from and written to
// 		//   Not Present: The page table is not present
// 		directory[i] = 0x0000000000000002;
// 	}
// }

// void idpaging(uint32_t *first_pte, vaddr from, int size) {
// 	from = from & 0xfffff000; // discard bits we don't want
// 	for(;size>0;from+=4096,size-=4096,first_pte++){
// 		*first_pte=from|1;     // mark page present.
// 	}
// }
