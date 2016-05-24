#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#include <stdarg.h>
#include <sysconsole.h>

void out_init();
int out_select(int terminal_desired);
void out_clear();

int out_printf(const char * fmt, ...);
int out_vprintf(const char * fmt, va_list arg);

void out_box_top();
void out_box_bottom();
void out_box_line(const char * fmt, ...);

void out_color_text(style_st color);
void out_color_bg(style_st color);

int out_cursor(cursor_st cursor);
void out_cursor_lock();

int out_sleep_init();
int out_sleep_step();
int out_sleep_stop();
int out_sleep_isActive();
int out_sleep_set(char * text, int boolean);
int out_sleep_reset();

#endif