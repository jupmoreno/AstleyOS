#ifndef _PORTS_H_
#define _PORTS_H_

#include <kdefine.h>

#define _PORT_COM1 0x3F8
#define _PORT_COM2 0x2F8
#define _PORT_COM3 0x3E8
#define _PORT_COM4 0x2E8

/**
 * TODO: Doc
 */
void _port_write_byte(uint16_t port, uint8_t data);
void _port_write_word(uint16_t port, uint16_t data);

uint8_t _port_read_byte(uint16_t port);
uint16_t _port_read_word(uint16_t port);

void _song_note(uint32_t frec, uint32_t length);

#endif