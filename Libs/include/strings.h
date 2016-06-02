#ifndef _LIB_STRINGS_H_
#define _LIB_STRINGS_H_

#include <define.h>

#define _CHAR_ISALPHA(x) (((x) >= 'a' && (x) <= 'z') || ((x) >= 'A' && (x) <= 'Z')) ? TRUE : FALSE
#define _CHAR_ISNUMBER(x) ((x) >= '0' && (x) <= '9') ? TRUE : FALSE

int isspace(char c);

size_t strlen(char * string);

/**
 * Compares 2 null terminated strings
 * @param  s1 a string
 * @param  s2 a string
 * @return    >0 if s1 is bigger, 0 if equal, <0 if smaller
 */
int strcmp(const char * s1, const char * s2);

/**
 * Copys the source string into destination
 * @param dest   a string
 * @param source a string
 */
void strcpy(char * dest, const char * source);

char * strchr(const char * s, int c);

char * strnum(int value, int base, char * result);

int strfnd(const char * string, char character);

#endif