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
extern cr3_t _cr4_read(void);
// extern void _cr4_write(cr4_t cr4);

static void pdpt_init(void);
static void pdt_init(pdte_st ** table, pte_st ** page_table, uint64_t * page);
static void pt_init(pte_st ** page_table, uint64_t * page);
static void paging_enable(void);

#define KILOBYTE 			1024				// TODO: En otro lado todos
#define KILOBYTES(x) 		((x) * KILOBYTE)
#define MEGABYTE 			1048576
#define MEGABYTES(x) 		((x) * MEGABYTE)

#define _MEMORY_PAGE_SIZE	KILOBYTES(4) // TODO: En otro lado

#define PDPT_BASE		0x0000000020740358
#define PDPT_ENTRIES	4
#define PDPT_SIZE		(PDPT_ENTRIES * sizeof(pdpte_st))
static pdpte_st * directory_pointers;
#define PDT_BASE		(PDPT_BASE + PDPT_SIZE)
#define PDT_ENTRIES		512
#define PDT_SIZE		(PDPT_ENTRIES * sizeof(pdte_st))
static pdte_st * page_directories;
#define PT_BASE			(PDT_BASE + PDT_SIZE)
#define PT_ENTRIES		512
#define PT_SIZE			(PDT_ENTRIES * sizeof(pte_st))
static pte_st * page_tables;

void paging_init(void) {
	log("<MEMORY> CR0: %h\n", _cr0_read());
	log("<MEMORY> CR3: %h\n", _cr3_read());
	log("<MEMORY> CR4: %h\n", _cr4_read());

	directory_pointers = (pdpte_st *) PDPT_BASE;
	page_directories = (pdte_st *) PDT_BASE;
	page_tables = (pte_st *) PT_BASE;

	pdpt_init();

	// paging_enable();
}

static void pdpt_init(void) {
	int i;
	uint64_t page = 0;
	pdte_st * directories = page_directories;
	pte_st * tables = page_tables;

	for(i = 0; i < PDPT_ENTRIES; i++) {
		pdt_init(&directories, &tables, &page);

		directory_pointers[i].p 		= 1;
		directory_pointers[i].zero_1 	= 0;
		directory_pointers[i].pwt 		= 0;
		directory_pointers[i].pcd 		= 1;
		directory_pointers[i].zero_2 	= 0;
		directory_pointers[i].zero_3 	= 0;
		directory_pointers[i].base 		= (uint64_t) directories;
		directory_pointers[i].zero_4 	= 0;
	}
}

static void pdt_init(pdte_st ** table, pte_st ** page_table, uint64_t * page) {
	int i;
	pdte_st * entry = *table;

	for(i = 0; i < PDT_ENTRIES; i++) {
		pt_init(page_table, page);

		entry->p		= 1;
		entry->rw		= 1;
		entry->us		= 1;
		entry->pwt		= 0;
		entry->pcd		= 1;
		entry->a 		= 1;
		entry->zero_1 	= 0;
		entry->ps 		= 0;
		entry->zero_2 	= 0;
		entry->base 	= (uint64_t) *page_table;
		entry->zero_3 	= 0;
		entry->xd 		= 0;

		entry += sizeof(pdte_st);
	}

	*table = entry;
}

static void pt_init(pte_st ** page_table, uint64_t * page) {
	int i;
	pte_st * entry = *page_table;

	for(i = 0; i < PT_ENTRIES; i++) {
		entry->p 		= 1;
		entry->rw 		= 1;
		entry->us 		= 1;
		entry->pwt 		= 0;
		entry->pcd 		= 1;
		entry->a 		= 1;
		entry->d 		= 0;
		entry->pat 		= 0;
		entry->g 		= 0;
		entry->zero_1 	= 0;
		entry->base 	= *page;
		entry->zero_2 	= 0;
		entry->xd 		= 0;

		(*page) += _MEMORY_PAGE_SIZE;
		entry += sizeof(pte_st);
	}

	*page_table = entry;
}

static void paging_enable(void) {
	cr0_t cr0;
	cr3_t cr3;
	// cr4_t cr4;

	// cr4 = _cr4_read();
	// cr4.pae = 1;
	// _cr4_write(cr4);

	cr3 = _cr3_read();
	cr3.zero_1 = 0;
	cr3.base = (uint64_t) directory_pointers;
	cr3.zero_2 = 0;
	_cr3_write(cr3);

	cr0 = _cr0_read();
	// cr0.pg = 1;
	_cr0_write(cr0);
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
