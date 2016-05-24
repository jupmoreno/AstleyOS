#include <serial.h>
#include <define.h>
#include <ports.h>

/*
Offset	DLAB	Register mapped to this port
+0		0		Data register. Reading this registers read from the Receive buffer. Writing to this register writes to the Transmit buffer.
+1		0		Interrupt Enable Register.
+0		1		With DLAB set to 1, this is the least significant byte of the divisor value for setting the baud rate.
+1		1		With DLAB set to 1, this is the most significant byte of the divisor value.
+2		-		Interrupt Identification and FIFO control registers
+3		-		Line Control Register. The most significant bit of this register is the DLAB.
+4		-		Modem Control Register.
+5		-		Line Status Register.
+6		-		Modem Status Register.
+7		-		Scratch Register.
*/

static int serial_transmit_isEmpty(void);
static int serial_received(void);

static int serial_initialized = FALSE;

// Original: http://wiki.osdev.org/Serial_Ports
void serial_init(void) {
	/* Notice that the initialization code writes to [PORT + 1] twice with different values. This is once to write to the Divisor register along with [PORT + 0] and once to write to the Interrupt register. The second write to the Line Control register [PORT + 3] clears the DLAB again as well as setting various other bits. */

	_port_write_byte(_PORT_COM1 + 1, 0x00);    // Disable all interrupts
	_port_write_byte(_PORT_COM1 + 3, 0x80);    // Enable DLAB (set baud rate divisor)
	_port_write_byte(_PORT_COM1 + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
	_port_write_byte(_PORT_COM1 + 1, 0x00);    //                  (hi byte)
	_port_write_byte(_PORT_COM1 + 3, 0x03);    // 8 bits, no parity, one stop bit
	_port_write_byte(_PORT_COM1 + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
	_port_write_byte(_PORT_COM1 + 4, 0x0B);    // IRQs enabled, RTS/DSR set

	serial_initialized = TRUE;
}

int serial_isInit(void) {
	return serial_initialized;
}

void serial_write(char character) {
	while(serial_transmit_isEmpty() == 0);

	_port_write_byte(_PORT_COM1, character);
}

void serial_print(char * string) {
	int i = 0;

	while(string[i]) {
		serial_write(string[i++]);
	}
}

char serial_read(void) {
	while (serial_received() == 0);

	return _port_read_byte(_PORT_COM1);
}

static int serial_transmit_isEmpty(void) {
	return _port_read_byte(_PORT_COM1 + 5) & 0x20;
}

static int serial_received(void) {
	return _port_read_byte(_PORT_COM1 + 5) & 1;
}