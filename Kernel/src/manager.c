#include <manager.h>
#include <define.h>
#include <limits.h>
#include <output.h>
#include <keyboard.h>
#include <rtc.h>
#include <syscalls.h>
#include <sysio.h>
#include <systime.h>
#include <stdarg.h>
#include <heap.h>
#include <sysalloc.h>

void manage_time(void);
void manage_key(key_st * key);
unsigned int manage_read(unsigned int fd, char * buffer, unsigned int length);
unsigned int manage_write(unsigned int fd, const char * string, unsigned int length);
int manage_rtc(int operation, time_st * time);
int manage_terminal(int operation, int value);
void * manage_alloc(int op, ...);

static unsigned int manage_read_stdin(char * buffer, unsigned int length);
static unsigned int manage_write_stdout(const char * string, unsigned int length);

static int manage_terminal_select(int index);
static int manage_terminal_clear(void);
static int manage_terminal_color(int operation, style_st color);
static int manage_terminal_cursor(cursor_st cursor);

static int key_print_enabled = TRUE;
static int key_buffer_enabled = TRUE;
static int terminal_isWriting = FALSE;

void manage_time(void) {

}

void manage_key(key_st * key) {
	if(key->type == _KEY_TYPE_PRINTABLE) {
		if(key_buffer_enabled) {
			if(key->value == '\b') {
				// If the key cannot be erased, nothing is done for now.
				keyboard_delete();
			} else {
				// If the key cannot be buffered, nothing is done for now.
				keyboard_write(key);
			}
		}

		if(key_print_enabled) {
			out_printf("%c", key->value);
		}
	}
}

unsigned int manage_read(unsigned int fd, char * buffer, unsigned int length) {
	switch(fd) {
		case _FILEDESCRIPTOR_STDIN:
			return manage_read_stdin(buffer, length);
	}

	return 0;
}

unsigned int manage_write(unsigned int fd, const char * string, unsigned int length) {
	switch(fd) {
		case _FILEDESCRIPTOR_STDOUT:
			return manage_write_stdout(string, length);
	}

	return 0;
}

int manage_rtc(int operation, time_st * time) {
	switch(operation) {
		case _TIME_OPERATION_SET:
			return rtc_write(time);

		case _TIME_OPERATION_GET:
			return rtc_read(time);
	}

	return _SYSCALL_ERROR_TIME_OPERATION_INVALID;
}

int manage_terminal(int operation, int value) {
	switch(operation) {
		case _TERMINAL_OPERATION_SELECT:
			return manage_terminal_select(value);

		case _TERMINAL_OPERATION_CLEAR:
			return manage_terminal_clear();

		case _TERMINAL_COLOR_OPERATION_STYLE:
		case _TERMINAL_COLOR_OPERATION_TEXT:
		case _TERMINAL_COLOR_OPERATION_BG:
			return manage_terminal_color(operation, (style_st) value);

		case _TERMINAL_OPERATION_CURSOR:
			return manage_terminal_cursor((cursor_st) value);
	}

	return _SYSCALL_ERROR_SCREEN_OPERATION_INVALID;
}

void * manage_alloc(int op, ...) {
	unsigned int size;
	void * addr;
	va_list arg;

	va_start(arg, op);

	switch(op) {
		case _ALLOC_NEW: {
			size = va_arg(arg, unsigned int);
			addr = heap_alloc(size);
		} break;

		case _ALLOC_FREE: {
			addr = va_arg(arg, void *);
			heap_free(addr);
		} break;

		// case _ALLOC_RENEW: {
		// 	addr = va_arg(arg, void *);
		// 	size = va_arg(arg, unsigned int);
		// 	addr = heap_realloc(addr, size);
		// } break;
	}

	va_end(arg);

	return addr;
}

static unsigned int manage_read_stdin(char * buffer, unsigned int length) {
	unsigned int i = 0;

	while(!keyboard_canRead());

	while(i < length && keyboard_canRead()) {
		buffer[i++] = keyboard_read();
	}

	return i;
}

static unsigned int manage_write_stdout(const char * string, unsigned int length) {
	unsigned int i = 0;
	int printed = 0;

	terminal_isWriting = TRUE;

	if(length == UINT_MAX) {
		printed = out_printf(string);
	} else {
		while(i < length && string[i]) {
			printed += out_printf("%c", string[i++]);
		}
	}

	out_cursor_lock();
	terminal_isWriting = FALSE;

	return printed;
}

static int manage_terminal_select(int index) {
	return out_select(index);
}

static int manage_terminal_clear(void) {
	out_clear();

	return OK;
}

static int manage_terminal_color(int operation, style_st color) {
	switch(operation) {
		case _TERMINAL_COLOR_OPERATION_STYLE:
			out_color_text(_VIDEO_COLOR_GETTER(color));
			out_color_bg(_VIDEO_BG_TO_COLOR(color));
			break;

		case _TERMINAL_COLOR_OPERATION_TEXT:
			out_color_text(_VIDEO_COLOR_GETTER(color));
			break;

		case _TERMINAL_COLOR_OPERATION_BG:
			out_color_bg(_VIDEO_COLOR_GETTER(color));
			break;

		default:
			return _SYSCALL_ERROR_SCREEN_OPERATION_INVALID;
	}

	return OK;
}

static int manage_terminal_cursor(cursor_st cursor) {
	return out_cursor(cursor);
}
