#ifndef _TERMINAL_H_
#define _TERMINAL_H_

#include <sysvideo.h>

typedef struct {
	unsigned int cursor;
	unsigned int cursor_lock;
	int cursor_shown;
	cursor_st cursor_shape;
	style_st style;
	screen_st screen;
} terminal_st;

void terminal_init(terminal_st * terminal);
void terminal_show(terminal_st * terminal);
void terminal_hide(void);
void terminal_clear(terminal_st * terminal);

int terminal_write(terminal_st * terminal, char character);
int terminal_print(terminal_st * terminal, char * string);
int terminal_digit(terminal_st * terminal, int number, unsigned int base);

void terminal_shift(terminal_st * terminal, int lines);

void terminal_color_text(terminal_st * terminal, style_st color);
void terminal_color_bg(terminal_st * terminal, style_st color);
int terminal_cursor(terminal_st * terminal, cursor_st shape);

void terminal_cursor_lock(terminal_st * terminal);

#endif