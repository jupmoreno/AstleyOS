#include <paging.h>
#include <define.h>
#include <log.h>
#include <pit.h>

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

typedef struct {
	uint64_t
		zero_1		: 3,	// Ignored
		pwt			: 1,	// Page-level Write-Through
		pcd			: 1,	// Page-level Cache Disable
		zero_2		: 7,	// Ignored
		base		: 40,	// Physical address of the 4-KByte aligned PML4 table
		zero_3		: 12;	// Ignored
} cr3_t;

typedef struct {
	uint64_t
		p			: 1,	// Present; must be 1 to reference a page table
		rw			: 1,	// Read/write
		us			: 1,	// User/supervisor
		pwt			: 1,	// Page-level write-through
		pcd			: 1,	// Page-level cache disable
		a			: 1,	// Accessed
		zero_1		: 1,	// Ignored
		ps			: 1,	// Reserved (must be 0)
		zero_2		: 4,	// Ignored
		base		: 40,	// Physical address of 4-KByte aligned page table
		zero_3		: 11,	// Ignored
		xd			: 1;	// Reserved (must be 0)
} lvl4e_st;

typedef struct {
	uint64_t
		p			: 1,	// Present; must be 1 to reference a page table
		rw			: 1,	// Read/write
		us			: 1,	// User/supervisor
		pwt			: 1,	// Page-level write-through
		pcd			: 1,	// Page-level cache disable
		a			: 1,	// Accessed
		zero_1		: 1,	// Ignored
		ps			: 1,	// Page size; must be 0 (otherwise, this entry maps a 1-GByte page)
		zero_2		: 4,	// Ignored
		base		: 40,	// Physical address of 4-KByte aligned page table
		zero_3		: 11,	// Ignored
		xd			: 1;	// Reserved (must be 0)
} lvl3e_st;

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
		zero_3		: 11,	// Ignored
		xd			: 1;	// Reserved (must be 0)
} lvl2e_st;

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
} lvl1e_st;

typedef struct {
	uint64_t
		offset		: 12,	// Offset
		table		: 9,	// Page table
		directory	: 9,	// Page Directory
		pointer 	: 9,	// Directory Pointer
		pml4 		: 9,	// Directory Pointer
		zero_1 		: 16;	// Directory Pointer
} address_st;

extern cr0_t _cr0_read(void);
extern void _cr0_write(cr0_t cr0);
extern cr3_t _cr3_read(void);
extern void _cr3_write(cr3_t cr3);
extern cr3_t _cr4_read(void);
// extern void _cr4_write(cr4_t cr4);

static void paging_enable(void);
static void lvl4t_init(uint64_t total);
static void lvl3t_init(uint64_t * base, uint64_t * size, uint64_t total, uint64_t * page_pointer);
static void lvl2t_init(uint64_t * base, uint64_t * size, uint64_t total, uint64_t * page_pointer);
static void lvl1t_init(uint64_t * base, uint64_t * size, uint64_t total, uint64_t * page_pointer);

#define KILOBYTE 		1024				// TODO: En otro lado todos
#define KILOBYTES(x) 	((x) * KILOBYTE)

#define _MEMORY_PAGE_SIZE_BYTES 4
#define _MEMORY_PAGE_SIZE		KILOBYTES(_MEMORY_PAGE_SIZE_BYTES) // TODO: En otro lado

#define TABLES_BASE		0x0000000020800000

#define LVL4_ENTRIES	512
#define LVL4_SIZE		(LVL4_ENTRIES * sizeof(lvl4e_st))

#define LVL3_ENTRIES	512
#define LVL3_SIZE		(LVL3_ENTRIES * sizeof(lvl3e_st))

#define LVL2_ENTRIES	512
#define LVL2_SIZE		(LVL2_ENTRIES * sizeof(lvl2e_st))

#define LVL1_ENTRIES	512
#define LVL1_SIZE		(LVL1_ENTRIES * sizeof(lvl1e_st))

void paging_init(void) {
	log("<MEMORY> CR0: %h\n", _cr0_read());
	log("<MEMORY> CR3: %h\n", _cr3_read());
	log("<MEMORY> CR4: %h\n", _cr4_read());

	lvl4t_init(250000); // TODO: Define 512000

	paging_enable();
}

static void lvl4t_init(uint64_t total) {
	int i;
	lvl4e_st * entry = (lvl4e_st *) TABLES_BASE;
	uint64_t base = TABLES_BASE + LVL4_SIZE;
	uint64_t page_pointer = 0, size = 0;

	for(i = 0; i < LVL4_ENTRIES; i++) {
		if(size < total) {
			log("Creo tabla lvl3 (%d)\n", i);
			// pit_wait(0.5);
			entry->p		= 1;
			entry->rw		= 1;
			entry->us		= 1;
			entry->pwt		= 0;
			entry->pcd		= 1;
			entry->a		= 1;
			entry->zero_1	= 0;
			entry->ps		= 0;
			entry->zero_2	= 0;
			entry->base		= (uint64_t)(base);
			entry->zero_3	= 0;
			entry->xd		= 0;

			lvl3t_init(&base, &size, total, &page_pointer);
		} else {
			entry->p		= 0;
		}
		entry += sizeof(lvl4e_st);
	}

	log("SIZE: %d\n", size);
}

static void lvl3t_init(uint64_t * base, uint64_t * size, uint64_t total, uint64_t * page_pointer) {
	int i;
	lvl3e_st * entry = (lvl3e_st *) *base;
	*base += LVL3_SIZE;

	for(i = 0; i < LVL3_ENTRIES; i++) {
		if(*size < total) {
			log("Creo tabla lvl2 (%d)\n", i);
			// pit_wait(0.5);
			entry->p		= 1;
			entry->rw		= 1;
			entry->us		= 1;
			entry->pwt		= 0;
			entry->pcd		= 1;
			entry->a		= 1;
			entry->zero_1	= 0;
			entry->ps		= 0;
			entry->zero_2	= 0;
			entry->base		= (uint64_t)(*base);
			entry->zero_3	= 0;
			entry->xd		= 0;

			lvl2t_init(base, size, total, page_pointer);
		} else {
			entry->p		= 0;
		}

		entry += sizeof(lvl3e_st);
	}
}

static void lvl2t_init(uint64_t * base, uint64_t * size, uint64_t total, uint64_t * page_pointer) {
	int i;
	lvl2e_st * entry = (lvl2e_st *) *base;
	*base += LVL2_SIZE;

	for(i = 0; i < LVL2_ENTRIES; i++) {
		if(*size < total) {
			log("Creo tabla lvl1 (%d)\n", i);
			// pit_wait(0.5);
			entry->p		= 1;
			entry->rw		= 0;
			entry->us		= 0;
			entry->pwt		= 0;
			entry->pcd		= 1;
			entry->a		= 0;
			entry->zero_1	= 0;
			entry->ps		= 0;
			entry->zero_2	= 0;
			entry->base		= (uint64_t)(*base);
			entry->zero_3	= 0;
			entry->xd		= 0;

			lvl1t_init(base, size, total, page_pointer);
		} else {
			entry->p		= 0;
		}

		entry += sizeof(lvl2e_st);
	}
}

static void lvl1t_init(uint64_t * base, uint64_t * size, uint64_t total, uint64_t * page_pointer) {
	int i;
	lvl1e_st * entry = (lvl1e_st *) *base;
	*base += LVL1_SIZE;

	for(i = 0; i < LVL1_ENTRIES; i++) {
		if(*size < total) {
			log("Mapeo pagina: %h\n", *page_pointer);
			// pit_wait(0.5);
			entry->p		= 1;
			entry->rw		= 1;
			entry->us		= 1;
			entry->pwt		= 0;
			entry->pcd		= 1;
			entry->a		= 1;
			entry->d		= 0;
			entry->pat		= 0;
			entry->g		= 0;
			entry->zero_1	= 0;
			entry->base		= *page_pointer;
			entry->zero_2	= 0;
			entry->xd		= 0;

			(*page_pointer) += (_MEMORY_PAGE_SIZE);
			(*size)++;
		} else {
			entry->p		= 0;
		}
		entry += sizeof(lvl1e_st);
	}
}

static void paging_enable(void) {
	// cr0_t cr0;
	cr3_t cr3;
	// cr4_t cr4;

	// cr4 = _cr4_read();
	// cr4.pae = 1;
	// _cr4_write(cr4);

	cr3 = _cr3_read();
	cr3.zero_1	= 0;
	// cr3.pwt		= 0;
	// cr3.pcd		= 1;
	cr3.zero_2	= 0;
	cr3.base	= TABLES_BASE;
	cr3.zero_3	= 0;
	_cr3_write(cr3);

	// cr0 = _cr0_read();
	// cr0.pg = 1;
	// _cr0_write(cr0);
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
