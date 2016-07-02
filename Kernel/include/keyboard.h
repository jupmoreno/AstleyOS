#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <kdefine.h>

#define _ERROR_KEYBOARD_INVALID -1
#define _ERROR_KEYBOARD_BUFFER_EMPTY -2
#define _ERROR_KEYBOARD_BUFFER_FULL -3
#define _ERROR_KEYBOARD_KEY_INVALID -4
#define _ERROR_KEYBOARD_OPERATION -5 // TODO: Change name

enum key_types {
	_KEY_TYPE_EXTENDED = 0,
	_KEY_TYPE_FUNCTIONAL,
	_KEY_TYPE_RELEASED,
	_KEY_TYPE_PRINTABLE,
	_KEY_TYPE_UNCLASSIFIED
};

#define _KEY_NOTPRINTABLE 0

enum keyboards {
	_KEYBOARD_SPANISH = 0,
	_KEYBOARD_ENGLISH
};

typedef struct {
	int type;
	/**
	 * if type == PRINTABLE --> value is an ASCII code
	 * else --> value is the defined scanned code.
	 */
	char value;
} key_st;

int keyboard_set(int keyboard);

int keyboard_write(key_st * key);
int keyboard_canRead(void);
char keyboard_read(void);
int keyboard_delete(void);
char canReadCurry(void);

#endif