#include <console.h>

int systerminal_select(int index);
void systerminal_clear(void);
void systerminal_color(int operation, style_st color);
int systerminal_cursor(cursor_st cursor);
void sysvideo_mode(void);

void clear(void) {
	systerminal_clear();
}

int sterminal(int index) {
	if(index < _OUTPUT_TERMINAL_MIN || index > _OUTPUT_TERMINAL_MAX) {
		return _OUTPUT_ERROR_TERMINAL_INVALID;
	}

	return systerminal_select(index);
}

void tterminal(style_st color) {
	systerminal_color(_TERMINAL_COLOR_OPERATION_TEXT, color);
}

void bgterminal(style_st color) {
	systerminal_color(_TERMINAL_COLOR_OPERATION_BG, color);
}

void stterminal(style_st style) {
	systerminal_color(_TERMINAL_COLOR_OPERATION_STYLE, style);
}
