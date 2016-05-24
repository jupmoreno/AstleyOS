#include <output.h>
#include <define.h>
#include <strings.h>
#include <numbers.h>
#include <terminal.h>

#define BOX_LINE_MAXLENGTH 76

#define SLEEP_TEXT_DEFAULT " ..|.. "

// Terminal vars
static terminal_st terminals[_OUTPUT_TERMINAL_SIZE] = {{0}, {0}};
static int terminal_active = _OUTPUT_TERMINAL_DEFAULT; // TODO: A futuro inicializar
// ^^^ Terminal vars ^^^

// Sleep vars
static int sleep_active = FALSE;
static char sleep_text[_VIDEO_SIZE + 1];
static int sleep_loop = TRUE;
// ^^^ Sleep vars ^^^

void out_init() {
	int i;

	for(i = _OUTPUT_TERMINAL_MIN; i < _OUTPUT_TERMINAL_SIZE; i++) {
		terminal_init(&terminals[i]);
	}

	terminal_active = _OUTPUT_TERMINAL_DEFAULT;
	terminal_show(&terminals[terminal_active]);

	strcpy(sleep_text, SLEEP_TEXT_DEFAULT); // TODO: Sacar cuando peuda alocar memoria
}

int out_select(int terminal_desired) {
	if(terminal_desired < _OUTPUT_TERMINAL_MIN || terminal_desired > _OUTPUT_TERMINAL_MAX) {
		return _OUTPUT_ERROR_TERMINAL_INVALID;
	}

	terminal_active = terminal_desired;
	terminal_show(&terminals[terminal_desired]);

	return OK;
}

void out_clear() {
	terminal_clear(&terminals[terminal_active]);
}

int out_printf(const char * fmt, ...) {
    int printed = 0;
    va_list arg;

    va_start(arg, fmt);
    printed = out_vprintf(fmt, arg);
	va_end(arg);

	return printed;
}

int out_vprintf(const char * fmt, va_list arg) { // TODO: How to set styles
    char symbol;
    int i = 0;
    int printed = 0;

    while(fmt[i] != 0) {
		if(fmt[i] == '%') {
			symbol = fmt[++i];
		    switch(symbol) {
		    	case 's':
					printed += terminal_print(&terminals[terminal_active], va_arg(arg, char *));
					break;
				case 'c':
					printed += terminal_write(&terminals[terminal_active], va_arg(arg, int));
					break;
				case 'd':
					printed += terminal_digit(&terminals[terminal_active], va_arg(arg, int), _NUMBERS_BASE_DEC);
					break;
				case 'h':
					printed += terminal_digit(&terminals[terminal_active], va_arg(arg, int), _NUMBERS_BASE_HEX);
					break;
				case 'b':
					printed += terminal_digit(&terminals[terminal_active], va_arg(arg, int), _NUMBERS_BASE_BIN);
					break;
				case '%':
					printed += terminal_write(&terminals[terminal_active], symbol);
					break;
			}
		} else {
			printed += terminal_write(&terminals[terminal_active], fmt[i]);
		}

		i++;
	}

	return printed;
}

void out_box_top() {
	out_printf("\n ------------------------------------------------------------------------------ ");
}

void out_box_bottom() {
	out_printf(" ------------------------------------------------------------------------------ ");
}

void out_box_line(const char * format, ...) {
	static int length;
	va_list args;

	va_start(args, format);

	out_printf("| ");
	length = out_vprintf(format, args);

	if(length > BOX_LINE_MAXLENGTH) {
		int aux = length - BOX_LINE_MAXLENGTH;
		length = BOX_LINE_MAXLENGTH;

		while(aux--) {
			out_printf("\b");
		}
	}

	length = BOX_LINE_MAXLENGTH - length;
	while(length--) {
		out_printf(" ");
	}
	out_printf(" |");

	va_end(args);
}

void out_color_text(style_st color) {
	terminal_color_text(&terminals[terminal_active], color);
}

void out_color_bg(style_st color) {
	terminal_color_bg(&terminals[terminal_active], color);
}

int out_cursor(cursor_st cursor) {
	return terminal_cursor(&terminals[terminal_active], cursor);
}

void out_cursor_lock() { // TODO: 
	terminal_cursor_lock(&terminals[terminal_active]);
}

// int out_sleep_isEnabled() { // TODO: En manager
// 	return sleep_enabled;
// }

// void out_sleep_enabled(int boolean) { // TODO: En manager
// 	if(sleep_active) {
// 		return _OUTPUT_ERROR_SLEEP_ACTIVE;
// 	}

// 	sleep_enabled = boolean ? TRUE : FALSE;
// }

// int out_sleep_time_set(unsigned int time) {
// 	if(sleep_active) {
// 		return _OUTPUT_ERROR_SLEEP_ACTIVE;
// 	}

// 	if(time < _OUTPUT_SLEEP_TIME_MIN) { // TODO: En manager
// 		return _OUTPUT_ERROR_SLEEP_TIME;
// 	}

// 	sleep_trigger = time;

// 	return OK;
// }

int out_sleep_init() { // TODO: Screen Saver
	if(sleep_active) {
		return _OUTPUT_ERROR_SLEEP_ACTIVE;
	}

	sleep_active = TRUE;
	terminal_hide();

	terminal_print(&terminals[terminal_active], sleep_text); // TODO: 

	return OK;
}

int out_sleep_step() {
	if(!sleep_active) {
		return _OUTPUT_ERROR_SLEEP_NOT_ACTIVE;
	}
	
	if(sleep_loop) {
		terminal_print(&terminals[terminal_active], sleep_text);
	}

	return OK;
}

int out_sleep_stop() {
	if(!sleep_active) {
		return _OUTPUT_ERROR_SLEEP_NOT_ACTIVE;
	}

	sleep_active = FALSE;

	return OK;
}

int out_sleep_isActive() {
	return sleep_active;
}

int out_sleep_set(char * text, int boolean) { // TODO: 
	if(sleep_active) {
		return _OUTPUT_ERROR_SLEEP_ACTIVE;
	}

	strcpy(sleep_text, text); // TODO: Sacar cuando pueda alocar memoria
	sleep_loop = boolean ? TRUE : FALSE;

	return OK;
}

int out_sleep_reset() {
	if(sleep_active) {
		return _OUTPUT_ERROR_SLEEP_ACTIVE;
	}

	strcpy(sleep_text, SLEEP_TEXT_DEFAULT); // TODO: Sacar cuando pueda alocar memoria
	sleep_loop = TRUE;

	return OK;
}
