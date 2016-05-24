#include <pic.h>
#include <ports.h>
#include <log.h>

/**
 * Some (if not all) code was adapted from: http://wiki.osdev.org/8259_PIC
 */

#define _PIC_LINES_MAX 8
#define _PIC_SIZE (_PIC_LINES_MAX * 2)

#define _PIC_EOI 0x20 // End-of-interrupt (EOI) command code

#define _PIC_MASTER_COMMAND 0x20
#define _PIC_MASTER_DATA (_PIC_MASTER_COMMAND + 1)
#define _PIC_SLAVE_COMMAND	0xA0
#define _PIC_SLAVE_DATA (_PIC_SLAVE_COMMAND + 1)

#define _PIC_IRR_READ 0x0A // OCW3 irq ready next CMD read
#define _PIC_ISR_READ 0x0B // OCW3 irq service next CMD read

static uint16_t pic_reg(int ocw3);

// TODO: PIC Remap
void pic_init() {
	#ifdef _DEGUB_ENABLED
	log("<PIC> Inited.\n");
	#endif
}

void pic_mask(uint8_t mask) {
	_port_write_byte(_PIC_MASTER_DATA, mask);

	#ifdef _DEGUB_ENABLED
	log("<PIC> Masked: %h\n", mask);
	#endif
}

void pic_mask_all() {
	pic_mask(_PIC_MASK_ALL);
}

int pic_irq_mask(uint8_t irq) {
	uint16_t port;
	uint8_t mask;

	if(irq < _PIC_LINES_MAX) {
		port = _PIC_MASTER_DATA;
	} if(irq < _PIC_SIZE) {
		port = _PIC_SLAVE_DATA;
		irq -= _PIC_LINES_MAX;
	} else {
		return _PIC_ERROR_IRQ_INVALID; // TODO: 
	}

	mask = _port_read_byte(port) | (1 << irq);
	_port_write_byte(port, mask);

	#ifdef _DEGUB_ENABLED
	log("<PIC> Masked: %h\n", mask);
	#endif

	return OK;
}

int pic_irq_clear(uint8_t irq) {
	uint16_t port;
	uint8_t mask;

	if(irq < _PIC_LINES_MAX) {
		port = _PIC_MASTER_DATA;
	} if(irq < _PIC_SIZE) {
		port = _PIC_SLAVE_DATA;
		irq -= _PIC_LINES_MAX;
	} else {
		return _PIC_ERROR_IRQ_INVALID; // TODO: 
	}

	mask = _port_read_byte(port) & ~(1 << irq);
	_port_write_byte(port, mask);

	#ifdef _DEGUB_ENABLED
	log("<PIC> Masked: %h\n", mask);
	#endif

	return OK;    
}
 
int pic_irq_eoi(uint8_t irq) {
	if(irq >= _PIC_SIZE) {
		return _PIC_ERROR_IRQ_INVALID; // TODO: 
	}

	if(irq >= _PIC_LINES_MAX) {
		_port_write_byte(_PIC_SLAVE_COMMAND, _PIC_EOI);
		// #ifdef _DEGUB_ENABLED
		// log("\n<PIC SLAVE EOI>\n");
		// #endif
	}

	_port_write_byte(_PIC_MASTER_COMMAND, _PIC_EOI);

// #ifdef _DEGUB_ENABLED
// 	log("\n<PIC MASTER EOI>\n");
// #endif

	return OK;
}

/* Returns the combined value of the cascaded PICs irq request register */
uint16_t pic_irr() {
    return pic_reg(_PIC_IRR_READ);
}
 
/* Returns the combined value of the cascaded PICs in-service register */
uint16_t pic_isr() {
    return pic_reg(_PIC_ISR_READ);
}

static uint16_t pic_reg(int ocw3) {
    /* OCW3 to PIC CMD to get the register values.  PIC2 is chained, and
     * represents IRQs 8-15.  PIC1 is IRQs 0-7, with 2 being the chain */
    _port_write_byte(_PIC_MASTER_COMMAND, ocw3);
    _port_write_byte(_PIC_SLAVE_COMMAND, ocw3);
    return (_port_read_byte(_PIC_SLAVE_COMMAND) << _PIC_LINES_MAX) | _port_read_byte(_PIC_MASTER_COMMAND);
}

// TODO: PIC Init & Remap
// /* reinitialize the PIC controllers, giving them specified vector offsets
//    rather than 8h and 70h, as configured by default */
 
// #define ICW1_ICW4	0x01		/* ICW4 (not) needed */
// #define ICW1_SINGLE	0x02		/* Single (cascade) mode */
// #define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
// #define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
// #define ICW1_INIT	0x10		/* Initialization - required! */
 
// #define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
// #define ICW4_AUTO	0x02		/* Auto (normal) EOI */
// #define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
// #define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
// #define ICW4_SFNM	0x10		/* Special fully nested (not) */
 
// /*
// arguments:
// 	offset1 - vector offset for master PIC
// 		vectors on the master become offset1..offset1+7
// 	offset2 - same for slave PIC: offset2..offset2+7
// */
// void PIC_remap(int offset1, int offset2)
// {
// 	unsigned char a1, a2;
 
// 	a1 = inb(PIC1_DATA);                        // save masks
// 	a2 = inb(PIC2_DATA);
 
// 	outb(PIC1_COMMAND, ICW1_INIT+ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
// 	io_wait();
// 	outb(PIC2_COMMAND, ICW1_INIT+ICW1_ICW4);
// 	io_wait();
// 	outb(PIC1_DATA, offset1);                 // ICW2: Master PIC vector offset
// 	io_wait();
// 	outb(PIC2_DATA, offset2);                 // ICW2: Slave PIC vector offset
// 	io_wait();
// 	outb(PIC1_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
// 	io_wait();
// 	outb(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
// 	io_wait();
 
// 	outb(PIC1_DATA, ICW4_8086);
// 	io_wait();
// 	outb(PIC2_DATA, ICW4_8086);
// 	io_wait();
 
// 	outb(PIC1_DATA, a1);   // restore saved masks.
// 	outb(PIC2_DATA, a2);
// }
