#ifndef _SERIAL_H_
#define _SERIAL_H_

void serial_init();
int serial_isInit();

void serial_write(char character);
void serial_print(char * string);

char serial_read();

#endif