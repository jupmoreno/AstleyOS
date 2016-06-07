#include <paging.h>
#include <log.h>
#include <heap.h>

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
		lvl1_entry	: 9,	// Page table
		lvl2_entry	: 9,	// Page Directory
		lvl3_entry	: 9,	// Directory Pointer
		lvl4_entry	: 9,	// Directory Pointer
		zero_1		: 16;	// Reserved (must be 0)
} address_st;

typedef struct {
	uint32_t
		p			: 1,
		rw			: 1,
		us			: 1,
		rsvd		: 1,
		id			: 1,
		zero_1		: 27;
} pferror_st;

extern cr0_t _cr0_read(void);
extern void _cr0_write(cr0_t cr0);
extern cr3_t _cr3_read(void);
extern void _cr3_write(cr3_t cr3);
extern cr3_t _cr4_read(void);
// extern void _cr4_write(cr4_t cr4);

static void paging_enable(void * addr);
static lvl4e_st * lvl4t_init(uint64_t pages_total);
static lvl3e_st * lvl3t_init(uint64_t * pages_done, uint64_t pages_total);
static lvl2e_st * lvl2t_init(uint64_t * pages_done, uint64_t pages_total);
static lvl1e_st * lvl1t_init(uint64_t * pages_done, uint64_t pages_total);

#define BASE_MASK(x) (((uint64_t)(x))>>12)

#define _MEMORY_PAGE_SIZE	0x1000 // TODO: En otro lado

#define LVL4_ENTRIES	512
#define LVL4_SIZE		(LVL4_ENTRIES * sizeof(lvl4e_st))

#define LVL3_ENTRIES	512
#define LVL3_SIZE		(LVL3_ENTRIES * sizeof(lvl3e_st))

#define LVL2_ENTRIES	512
#define LVL2_SIZE		(LVL2_ENTRIES * sizeof(lvl2e_st))

#define LVL1_ENTRIES	512
#define LVL1_SIZE		(LVL1_ENTRIES * sizeof(lvl1e_st))

void paging_init(void) {
	lvl4e_st * lvl4_table;

	log("<PAGING> CR0: %h\n", _cr0_read());
	log("<PAGING> CR3: %h\n", _cr3_read());

	lvl4_table = lvl4t_init(262144); // TODO: Define
	if(lvl4_table == NULL) {
		return; // TODO: Handle errors
	}

	paging_enable(lvl4_table);
}

void pferror_handler(uint64_t code, uint64_t fault_address) {
	log("-----------------------> ERROR");
}

static lvl4e_st * lvl4t_init(uint64_t pages_total) {
	int i;
	uint64_t pages_done = 0;
	lvl4e_st * entry, * lvl4_table;
	lvl3e_st * lvl3_table;

	lvl4_table = heap_alloc(LVL4_SIZE);
	if(lvl4_table == NULL) {
		return NULL;
	}

	entry = lvl4_table;

	for(i = 0; i < LVL4_ENTRIES; i++) {
		if(pages_done < pages_total) {
			lvl3_table = lvl3t_init(&pages_done, pages_total);
			if(lvl3_table == NULL) {
				return NULL; // TODO: Free old elements
			}

			entry->p		= 1;
			entry->rw		= 1;
			entry->us		= 1;
			entry->pwt		= 0;
			entry->pcd		= 0;
			entry->a		= 1;
			entry->zero_1	= 0;
			entry->ps		= 0;
			entry->zero_2	= 0;
			entry->base		= BASE_MASK(lvl3_table);
			entry->zero_3	= 0;
			entry->xd		= 0;

			log("<PAGING> (LVL4) Entry #%d: %h:%h\n", i, lvl3_table, entry->base);
		} else {
			entry->p		= 0;
		}

		entry += sizeof(lvl4e_st);
	}

	return lvl4_table;
}

static lvl3e_st * lvl3t_init(uint64_t * pages_done, uint64_t pages_total) {
	int i;
	lvl3e_st * entry, * lvl3_table;
	lvl2e_st * lvl2_table;

	lvl3_table = heap_alloc(LVL3_SIZE);
	if(lvl3_table == NULL) {
		return NULL;
	}

	entry = lvl3_table;

	for(i = 0; i < LVL3_ENTRIES; i++) {
		if(*pages_done < pages_total) {
			lvl2_table = lvl2t_init(pages_done, pages_total);
			if(lvl2_table == NULL) {
				return NULL; // TODO: Free old elements
			}

			entry->p		= 1;
			entry->rw		= 1;
			entry->us		= 1;
			entry->pwt		= 0;
			entry->pcd		= 0;
			entry->a		= 1;
			entry->zero_1	= 0;
			entry->ps		= 0;
			entry->zero_2	= 0;
			entry->base		= BASE_MASK(lvl2_table);
			entry->zero_3	= 0;
			entry->xd		= 0;

			log("<PAGING> (LVL3) Entry #%d: %h:%h\n", i, lvl2_table, entry->base);
		} else {
			entry->p		= 0;
		}

		entry += sizeof(lvl3e_st);
	}

	return lvl3_table;
}

static lvl2e_st * lvl2t_init(uint64_t * pages_done, uint64_t pages_total) {
	int i;
	lvl2e_st * entry, * lvl2_table;
	lvl1e_st * lvl1_table;

	lvl2_table = heap_alloc(LVL2_SIZE);
	if(lvl2_table == NULL) {
		return NULL;
	}

	entry = lvl2_table;

	for(i = 0; i < LVL2_ENTRIES; i++) {
		if(*pages_done < pages_total) {
			lvl1_table = lvl1t_init(pages_done, pages_total);
			if(lvl1_table == NULL) {
				return NULL; // TODO: Free old elements
			}

			entry->p		= 1;
			entry->rw		= 1;
			entry->us		= 1;
			entry->pwt		= 0;
			entry->pcd		= 0;
			entry->a		= 1;
			entry->zero_1	= 0;
			entry->ps		= 0;
			entry->zero_2	= 0;
			entry->base		= BASE_MASK(lvl1_table);
			entry->zero_3	= 0;
			entry->xd		= 0;

			log("<PAGING> (LVL2) Entry #%d: %h:%h\n", i, lvl1_table, entry->base);
		} else {
			entry->p		= 0;
		}

		entry += sizeof(lvl2e_st);
	}

	return lvl2_table;
}

static lvl1e_st * lvl1t_init(uint64_t * pages_done, uint64_t pages_total) {
	int i;
	lvl1e_st * entry, * lvl1_table;

	lvl1_table = heap_alloc(LVL1_SIZE);
	if(lvl1_table == NULL) {
		return NULL;
	}

	entry = lvl1_table;

	for(i = 0; i < LVL1_ENTRIES; i++) {
		if(*pages_done < pages_total) {
			entry->p		= 1;
			entry->rw		= 1;
			entry->us		= 1;
			entry->pwt		= 1;
			entry->pcd		= 0;
			entry->a		= 1;
			entry->d		= 1;
			entry->pat		= 0;
			entry->g		= 0;
			entry->zero_1	= 0;
			entry->base		= (*pages_done);
			entry->zero_2	= 0;
			entry->xd		= 0;

			// log("<PAGING> (LVL1) Entry #%d: %h:%h\n", i, (*pages_done), entry->base);

			(*pages_done) = (*pages_done) + 1;
		} else {
			entry->p		= 0;
		}

		entry += sizeof(lvl1e_st);
	}

	return lvl1_table;
}

static void paging_enable(void * table) {
	cr3_t cr3;

	cr3 = _cr3_read();
	cr3.zero_1 = 0;
	// cr3.pwt = 1;
	// cr3.pcd = 0;
	cr3.zero_2 = 0;
	cr3.base = BASE_MASK(table);
	cr3.zero_3 = 0;
	log("<PAGING> NEW CR3: %d\n", cr3);
	_cr3_write(cr3);
}
