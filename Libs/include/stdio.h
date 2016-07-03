#ifndef _LIB_STDIO_H_
#define _LIB_STDIO_H_

#include <define.h>
#include <sysio.h>
#include <stdarg.h>

#define CLEAR_SCAN_BUFFER while(scanc() != '\n')

int scanc(void);
int scant(char * buffer, int size, const char end[], int endc);
int scanw(char * buffer, int size);
int scanl(char * buffer, int size);
// TODO:
// int scanf(const char * fmt, ...);
// int vscanf(const char * fmt, va_list arg);

int printc(char character);
int prints(char * string);
int printn(int number, unsigned int base);
int printf(const char * fmt, ...);
int vprintf(const char * fmt, va_list arg);

int yesno(const char msg[], ...);

void milisecSleep(int milisec);

#endif