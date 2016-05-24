#include <strings.h>
#include <stdint.h>

int isspace(char c) {
	return (c == ' ' || c == '\t' || c == '\n');
}

size_t strlen(char * string) { // TODO: Unsigned?
	unsigned int i = 0;

	while(string[i]) {
		i++;
	}

	return i;
}

int strcmp(const char * s1, const char * s2) {
    while(*s1 && (*s1 == *s2)) {
        s1++, s2++;
    }

    return *(const unsigned char *) s1 - *(const unsigned char *) s2;
}

void strcpy(char * dest, const char * source) {
	int i = 0;

    while((dest[i] = source[i]) != '\0') {
    	i++;
    }
}

// http://www.opensource.apple.com/source/gcc/gcc-937.2/libiberty/strchr.c
char * strchr(const char * s, int c) {
	do {
		if(*s == c) {
			return (char *) s;
		}
	} while (*s++);

	return NULL;
}

/*
** C++ version 0.4 char* style "itoa":
** Written by Lukás Chmela
** Released under GPLv3.
** URL: http://www.strudel.org.uk/itoa/
*/
char * strnum(int value, int base, char * result) {
	char * ptr = result, * ptr1 = result, tmp_char;
	int tmp_value;

	// Check that the base is valid
	if(base < 2 || base > 36) {
		*result = '\0'; 
		return result;
	}

	do {
		tmp_value = value;
		value /= base;
		*ptr++ = "ZYXWVUTSRQPONMLKJIHGFEDCBA9876543210123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ" [35 + (tmp_value - value * base)];
	} while(value);

	// Apply negative sign
	if(tmp_value < 0) {
		*ptr++ = '-';
	}

	// Terminate string
	*ptr-- = '\0';

	// Reverse string
	while(ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr-- = *ptr1;
		*ptr1++ = tmp_char;
	}

	return result;
}

int strfnd(const char * string, char character) {
	int i;

	for(i = 0; string[i] != 0; i++) {
		if(string[i] == character) {
			return i;
		}
	}

	return -1;
}
