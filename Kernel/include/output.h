#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#include <stdarg.h>
#include <sysconsole.h>

void out_init(void);
int out_select(int terminal_desired);
void out_clear(void);

int out_printf(const char * fmt, ...);
int out_vprintf(const char * fmt, va_list arg);

void out_box_top(void);
void out_box_bottom(void);
void out_box_line(const char * fmt, ...);

void out_color_text(style_st color);
void out_color_bg(style_st color);

int out_cursor(cursor_st cursor);
void out_cursor_lock(void);

int out_sleep_init(void);
int out_sleep_step(void);
int out_sleep_stop(void);
int out_sleep_isActive(void);
int out_sleep_set(char * text, int boolean);
int out_sleep_reset(void);

#endif