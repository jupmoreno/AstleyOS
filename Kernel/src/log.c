#include <log.h>
#include <stdarg.h>
#include <serial.h>
#include <strings.h>
#include <numbers.h>

#define CONVERT_BUFFER_SIZE 128
static char convert_buffer[CONVERT_BUFFER_SIZE] = {0}; // TODO: Cambiar de lugar!!

static void log_write(char character);
static int log_print(char * string);
static void log_digit(int number, unsigned int base);

void log_init(void) {
	if(!serial_isInit()) {
		serial_init();
	}
}

int log(const char * fmt, ...) { // TODO: How to set styles
	#ifdef _DEBUG_ENABLED
    char symbol;
    int i = 0;
    va_list arg;

    va_start(arg, fmt);
    while(fmt[i] != 0) {
		if(fmt[i] == '%') {
			symbol = fmt[++i];
		    switch(symbol) {
		    	case 's':
					log_print(va_arg(arg, char *));
					break;
				case 'c':
					log_write(va_arg(arg, int));
					break;
				case 'd':
					log_digit(va_arg(arg, int), _NUMBERS_BASE_DEC);
					break;
				case 'h':
					log_digit(va_arg(arg, int), _NUMBERS_BASE_HEX);
					break;
				case 'b':
					log_digit(va_arg(arg, int), _NUMBERS_BASE_BIN);
					break;
				case '%':
					log_write(symbol);
					break;
			}
		} else {
			log_write(fmt[i]);
		}

		i++;
	}
	va_end(arg);

	return i - 1;
	#else
	return 0;
	#endif
}

static void log_write(char character) {
	serial_write(character);
}

static int log_print(char * string) {
	int i = 0;

	while(string[i]) {
		log_write(string[i++]);
	}

	return i;
}

static void log_digit(int number, unsigned int base) {
	log_print(strnum(number, base, convert_buffer));

	switch(base) {
		case _NUMBERS_BASE_BIN:
			log_write('b');
			break;
		case _NUMBERS_BASE_HEX:
			log_write('h');
			break;
		default:
			break;
	}
}
