#ifndef _VIDEO_H_
#define _VIDEO_H_

#include <kdefine.h>
#include <sysvideo.h>

void video_init(void);
void video_clear(void);

void video_cursor_show(int boolean);
int video_cursor_put(unsigned int position);
int video_cursor_get(void);

int video_cursor_shape(cursor_st shape);
int video_cursor_shape_isValid(cursor_st shape);

int video_write(unsigned int position, char character);
int video_writeWithStyle(unsigned int position, char character, style_st style);
char video_get(unsigned int position);

int video_style_put(unsigned int position, style_st style);
int video_style_get(unsigned int position);
void video_style_all(style_st style);
int video_style_range(unsigned int from, unsigned int to, style_st style);

int video_color_put(unsigned int position, style_st color);
int video_color_get(unsigned int position);
void video_color_all(style_st color);
int video_color_range(unsigned int from, unsigned int to, style_st color);

int video_bg_put(unsigned int position, style_st bg);
int video_bg_get(unsigned int position);
void video_bg_all(style_st bg);
int video_bg_range(unsigned int from, unsigned int to, style_st bg);

#endif
