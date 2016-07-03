#include <stdio.h>
#include <limits.h>
#include <strings.h>
#include <numbers.h>

#define CONVERT_BUFFER_SIZE 128 // TODO: Cambiar cuando pueda alocar memoria?
static char convert_buffer[CONVERT_BUFFER_SIZE] = {0}; // TODO: Cambiar cuando pueda alocar memoria?

extern unsigned int sysread(char * buffer, unsigned int length);
extern unsigned int syswrite(char * string, unsigned int length);
extern void sys_sleep(uint32_t milisec);

static int matches(char c, const char chars[], int charc);

int scanc(void) {
	char c;
	int ret = sysread(&c, 1);

	return ret != EOF ? c : ret;
}

int scant(char * buffer, int size, const char end[], int endc) {
	int i = 0, found, c;

	if(size < 1/* || endc < 1*/) { // TODO: Remove?
		return -1;
	}

	while(i < (size - 1) && (c = scanc()) != EOF && !(found = matches(c, end, endc))) {
		buffer[i++] = c;
	}

	buffer[i] = found ? c : '\0';

	return i;
}

int scanw(char * buffer, int size) {
	static const char end[] = {' ', '\n'};
	int ret;

	while((ret = scant(buffer, size, end, 2)) != -1 && buffer[0] == ' ');

	return ret;
}

int scanl(char * buffer, int size) {
	static const char end[] = {'\n'};

	return scant(buffer, size, end, 1);
}

// // TODO: Test!
// int scanf(const char * fmt, ...) {
//     va_list arg;

//     va_start(arg, fmt);
//     vscanf(fmt, arg);
// 	va_end(arg);

// 	return OK; // TODO: 
// }

// // TODO: Test!
// int vscanf(const char * fmt, va_list arg) {
// 	char * c;
// 	int * n;
// 	int ret;

// 	while(*fmt) {
// 		while(isspace(*fmt)) fmt++;
// 		if(!(*fmt)) break;

// 		if(*fmt == '%') {
// 			fmt++;

// 			switch(*fmt) {
// 				case 'c':
// 					c = va_arg(arg, char *);
// 					*c = scanc();
// 					break;
// 				case 's':
// 					c = va_arg(arg, char *);
// 					while((ret = scanc()) != EOF && !isspace(ret) /*&& i < size*/) {
// 						*c = ret;
// 						c++;
// 					}
// 					*c = 0;
// 					break;
// 				// case 'd':
// 				// 	n = va_arg(arg, int *);;
// 				// 	break;
// 				default:
// 					break;
// 			}
// 		}

// 		fmt++;
// 	}

// 	return OK; // TODO: 
// }


int printc(char character) {
	return syswrite(&character, 1);
}

int prints(char * string) {
	return syswrite(string, UINT_MAX);
}

int printn(int number, unsigned int base) {
	int printed = prints(strnum(number, base, convert_buffer));

	switch(base) {
		case _NUMBERS_BASE_BIN:
			printed += printc('b');
			break;
		case _NUMBERS_BASE_HEX:
			printed += printc('h');
			break;
		default:
			break;
	}

	return printed;
}

int printf(const char * fmt, ...) {
    int printed = 0;
    va_list arg;

    va_start(arg, fmt);
    printed = vprintf(fmt, arg);
	va_end(arg);

	return printed;
}

int vprintf(const char * fmt, va_list arg) { // TODO: How to set styles
    char symbol;
    int i = 0;
    int printed = 0;

    while(fmt[i] != 0) {
		if(fmt[i] == '%') {
			symbol = fmt[++i];
		    switch(symbol) {
		    	case 's':
					printed += prints(va_arg(arg, char *));
					break;
				case 'c':
					printed += printc(va_arg(arg, int));
					break;
				case 'd':
					printed += printn(va_arg(arg, int), _NUMBERS_BASE_DEC);
					break;
				case 'h':
					printed += printn(va_arg(arg, int), _NUMBERS_BASE_HEX);
					break;
				case 'b':
					printed += printn(va_arg(arg, int), _NUMBERS_BASE_BIN);
					break;
				case '%':
					printed += printc(symbol);
					break;
			}
		} else {
			printed += printc(fmt[i]);
		}

		i++;
	}

	return printed;
}

// Usage:
// do {
// 	yes = yesno("Seguir?")
// } while(yes == -1);
// TODO: Optimize?
int yesno(const char msg[], ...) {
	int op, ret;
	va_list arg;
		
	va_start(arg, msg);
	vprintf(msg, arg);
	va_end(arg);

	op = scanc();
	if(strchr("Yy", op) != NULL) {
		ret = TRUE;
	} else if(strchr("Nn", op) != NULL) {
		ret = FALSE;
	} else {
		ret = -1;
	}

	if(op != '\n') {
		CLEAR_SCAN_BUFFER;
	}

	return ret;
}

static int matches(char c, const char chars[], int charc) {
	int i;

	for(i = 0; i < charc; i++) {
		if(c == chars[i]) {
			return TRUE;
		}
	}

	return FALSE;
}

void milisecSleep(int milisec){
	sys_sleep(milisec);
}
