#ifndef _LIB_NUMBERS_H_
#define _LIB_NUMBERS_H_

#define _NUMBERS_BASE_BIN 2
#define _NUMBERS_BASE_DEC 10
#define _NUMBERS_BASE_HEX 16

#define ABS(x) ((x) < 0) ? (-(x)) : (x)

int numstr(const char * string);
int numstrl(const char * string, int * value);

#endif