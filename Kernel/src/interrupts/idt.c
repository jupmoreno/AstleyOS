#include <idt.h>

#define IDT_BASE_ADDRESS 0x0000000000000000

#define WORD_GET(x) ((x) & 0xFFFF) // TODO: En kdefine.h?
#define WORD_BITS 16 // TODO: En kdefine.h?

// Segment Descriptor // TODO: Remove?
typedef struct {
	word_t 	limit,
			base_l;		// Low
	byte_t 	base_m,		// Mid
       		access,
       		attributes,
       		base_h;		// High
} __attribute__((packed)) segment_st;

// Interrupt Descriptor
typedef struct {
	word_t 	offset_l,	// Low
			selector;
	byte_t 	zero_l,		// Low
			access;
	word_t	offset_m;	// Mid
	dword_t	offset_h;	// High
	dword_t	zero_h;		// High
} __attribute__((packed)) interrupt_st;

// IDTR // TODO:
typedef struct {
	word_t	limit;
	qword_t	base;
} __attribute__((packed)) idtr_st;

static interrupt_st * idt = (interrupt_st *) IDT_BASE_ADDRESS;
static idtr_st idtr;

void idt_init(void) {
	// idt = heap_alloc(_IDT_ENTRIES * sizeof(interrupt_st));
	// if(idt == NULL) {
	// 	return; // TODO: Handle errors
	// }

	idtr.limit = _IDT_ENTRIES * sizeof(interrupt_st) - 1;
	idtr.base = (qword_t) idt;

	// TODO: Set idtr
}

int idt_entry(unsigned int index, qword_t offset, byte_t access) {
	if(index >= _IDT_ENTRIES) {
		return FALSE;
	}

	idt[index].selector = _GDT_CODE_SEGMENT;
	idt[index].offset_l = WORD_GET(offset);
	offset >>= WORD_BITS;
	idt[index].offset_m = WORD_GET(offset);
	offset >>= WORD_BITS;
	idt[index].offset_h = WORD_GET(offset);
	idt[index].access = access;
	idt[index].zero_l = idt[index].zero_h = 0;

	return TRUE;
}