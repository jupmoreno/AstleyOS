#include <terminal.h>
#include <define.h>
#include <video.h>
#include <stdarg.h>
#include <numbers.h>
#include <strings.h>

#define TAB_SIZE 3

#define IS_CURSOR_AT_BEGGINING_OF_LINE(x) (!((x) % _VIDEO_COLUMNS))

static void terminal_writter(terminal_st * terminal, char character);
static int terminal_newline(terminal_st * terminal);
static int terminal_tab(terminal_st * terminal);
static int terminal_delete(terminal_st * terminal);

#define CONVERT_BUFFER_SIZE 128 // TODO: Cambiar cuando pueda alocar memoria?
static char convert_buffer[CONVERT_BUFFER_SIZE] = {0}; // TODO: Cambiar cuando pueda alocar memoria?

static terminal_st * terminal_active;

void terminal_init(terminal_st * terminal) {
	int i;

	terminal->cursor = 0;
	terminal->cursor_lock = 0;
	terminal->cursor_shown = TRUE;
	terminal->style = _VIDEO_STYLE_DEFAULT;
	terminal->cursor_shape = _VIDEO_CURSOR_DEFAULT;

	for(i = 0; i < _VIDEO_SIZE; i++) {
		terminal->screen[i].character = ' ';
		terminal->screen[i].style = terminal->style;
	}
}

void terminal_show(terminal_st * terminal) {
	int i;

	// Send to video
	for(i = 0; i < _VIDEO_SIZE; i++) {
		pixel_st pixel = terminal->screen[i];
		video_writeWithStyle(i, pixel.character, pixel.style);
	}

	video_cursor_put(terminal->cursor);
	video_cursor_show(terminal->cursor_shown);
	video_cursor_shape(terminal->cursor_shape);
	terminal_active = terminal;
}

void terminal_hide() {
	video_clear();
	video_cursor_show(FALSE);
}

void terminal_clear(terminal_st * terminal) {
	int i;

	terminal->cursor = 0;
	terminal->cursor_lock = 0;

	for(i = 0; i < _VIDEO_SIZE; i++) {
		terminal->screen[i].character = ' ';
		terminal->screen[i].style = terminal->style; // TODO: Style tmb??
		video_writeWithStyle(i, ' ', terminal->style);
	}

	video_cursor_put(terminal->cursor);
}

int terminal_write(terminal_st * terminal, char character) {
	int printed = 0;

	switch(character) {
		case '\n': // New line
			printed = terminal_newline(terminal);
			break;

		case '\t': // Tab
			printed = terminal_tab(terminal);
			break;

		case '\b': // Backspace
			printed = terminal_delete(terminal) ? -1 : 0;
			break;

		default: // Normal Character
			terminal_writter(terminal, character);
			printed = 1;
			break;
	}

	return printed;
}

int terminal_print(terminal_st * terminal, char * string) {
	int i = 0;
	int printed = 0;

	while(string[i]) {
		printed += terminal_write(terminal, string[i++]);
	}

	return printed;
}

int terminal_digit(terminal_st * terminal, int number, unsigned int base) {
	int printed = terminal_print(terminal, strnum(number, base, convert_buffer));

	switch(base) {
		case _NUMBERS_BASE_BIN:
			printed += terminal_write(terminal, 'b');
			break;
		case _NUMBERS_BASE_HEX:
			printed += terminal_write(terminal, 'h');
			break;
		default:
			break;
	}

	return printed;
}

void terminal_shift(terminal_st * terminal, int lines) { // TODO: Por ahora no va a hacer nada con lines
	int i, j;

	pixel_st pixel;
	//int actual, next; // TODO: Choice
	
	// Move each row to the previous, the first one disappears
	for(i = 1; i < _VIDEO_ROWS; i++) {
		for(j = 0; j < _VIDEO_COLUMNS; j++) {
			pixel.character = terminal->screen[_VIDEO_CURSOR_TO_POSITION(i, j)].character;
			pixel.style = terminal->screen[_VIDEO_CURSOR_TO_POSITION(i, j)].style;

			terminal->screen[_VIDEO_CURSOR_TO_POSITION(i - 1, j)].character = pixel.character;
			terminal->screen[_VIDEO_CURSOR_TO_POSITION(i - 1, j)].style = pixel.style;

			if(terminal == terminal_active) {
				video_writeWithStyle(_VIDEO_CURSOR_TO_POSITION(i - 1, j), pixel.character, pixel.style);
			}
		}
	}

	// Clear last row
	for(j = 0; j < _VIDEO_COLUMNS; j++) {
		terminal->screen[_VIDEO_CURSOR_TO_POSITION(_VIDEO_ROWS - 1, j)].character = ' ';
		terminal->screen[_VIDEO_CURSOR_TO_POSITION(_VIDEO_ROWS - 1, j)].style = terminal->style;

		if(terminal == terminal_active) {
			video_writeWithStyle(_VIDEO_CURSOR_TO_POSITION(_VIDEO_ROWS - 1, j), ' ', terminal->style);
		}
	}

	// Update cursor position
	terminal->cursor -= _VIDEO_COLUMNS;
	
	if(terminal == terminal_active) {
		video_cursor_put(terminal->cursor);
	}
}

void terminal_color_text(terminal_st * terminal, style_st color) {
	int i;

	for(i = 0; i < _VIDEO_SIZE; i++) {
		terminal->screen[i].style = (terminal->style & 0xF0) + (color & 0x0F); // TODO: 
		video_color_put(i, color);
	}
}

void terminal_color_bg(terminal_st * terminal, style_st color) {
	int i;

	for(i = 0; i < _VIDEO_SIZE; i++) {
		terminal->screen[i].style = (terminal->style & 0x0F) + (color & 0xF0); // TODO: 
		video_bg_put(i, color);
	}
}

int terminal_cursor(terminal_st * terminal, cursor_st shape) {
	if(!video_cursor_shape_isValid(shape)) {
		return _VIDEO_ERROR_CURSOR_SHAPE_INVALID;
	}

	terminal->cursor_shape = shape;

	if(terminal == terminal_active) {
		return video_cursor_shape(shape);
	}

	return OK;
}

void terminal_cursor_lock(terminal_st * terminal) {
	terminal->cursor_lock = terminal->cursor;
}

static void terminal_writter(terminal_st * terminal, char character) {
	// If cursor is in last position -> shifts up the page (1)
	if(terminal->cursor == _VIDEO_CURSOR_LAST_POSITION) {
		terminal_shift(terminal, 1); // TODO: Or -1
	}

	terminal->screen[terminal->cursor].character = character;
	terminal->screen[terminal->cursor].style = terminal->style;

	if(terminal == terminal_active) {
		video_writeWithStyle(terminal->cursor, character, terminal->style);
		video_cursor_put(terminal->cursor + 1);
	}

	terminal->cursor++;
}

static int terminal_newline(terminal_st * terminal) {
	int printed = 0;

	if(IS_CURSOR_AT_BEGGINING_OF_LINE(terminal->cursor)) {
		terminal_writter(terminal, ' ');
		printed++;
	}

	while(!IS_CURSOR_AT_BEGGINING_OF_LINE(terminal->cursor)) {
		terminal_writter(terminal, ' ');
		printed++;
	}

	return printed;
}

static int terminal_tab(terminal_st * terminal) {
	int printed;

	for(printed = 0; printed < TAB_SIZE; printed++) {
		terminal_writter(terminal, ' ');
	}

	return printed;
}

static int terminal_delete(terminal_st * terminal) {
	if(terminal->cursor <= terminal->cursor_lock || terminal->cursor == 0) { // TODO: Mejor forma?
		return FALSE;
	}

	terminal->cursor--;
	terminal->screen[terminal->cursor].character = ' ';
	terminal->screen[terminal->cursor].style = terminal->style;

	if(terminal == terminal_active) {
		video_writeWithStyle(terminal->cursor, ' ', terminal->style);
		video_cursor_put(terminal->cursor);
	}

	return TRUE;
}
