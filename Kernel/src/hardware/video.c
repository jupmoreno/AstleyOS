#include <video.h>
#include <ports.h>

// DEFINES
#define VIDEO_DIR 0xB8000

#define PORT_VIDEO_INDEX 0x3D4
#define PORT_VIDEO_DATA 0x3D5

// Cursor Registers
#define INDEX_CURSOR_START_REGISTER 0x0A
#define INDEX_CURSOR_END_REGISTER 0x0B
#define INDEX_CURSOR_LOCATION_HIGH_REGISTER 0x0E
#define INDEX_CURSOR_LOCATION_LOW_REGISTER 0x0F
// ^^^ Cursor Registers ^^^

// Cursor Shapes
// Square
#define CURSOR_SHAPE_SQUARE_START 0x00
#define CURSOR_SHAPE_SQUARE_END 0x1F
// Underscore
#define CURSOR_SHAPE_UNDERSCORE_START 0x0E
#define CURSOR_SHAPE_UNDERSCORE_END 0x1F
// Dot
#define CURSOR_SHAPE_DOT_START 0x07
#define CURSOR_SHAPE_DOT_END 0x1F
// ^^^ Cursor Shapes ^^^

#define GET_LAST_5BITS(x) ((x) & 0x01F)
// ^^^ DEFINES ^^^
 
// LOCAL FUNCTIONS
/**
 * TODO: Docs
 * @param start [description]
 * @param end   [description]
 */
static void video_cursor_shape_changer(uint8_t start, uint8_t end);

/**
 * TODO: Docs
 */
static void video_cursor_setter(unsigned int position);
// ^^^ LOCAL FUNCTIONS ^^^

// VARIABLES
static pixel_st * video = (pixel_st *) VIDEO_DIR; // TODO: pixel_st * O screen_st???
static int cursor = 0;
static int cursor_shown = TRUE;
// ^^^ VARIABLES ^^^

void video_init() {
	cursor_shown = TRUE;
	video_cursor_shape(_VIDEO_CURSOR_DEFAULT);
	video_clear();
}

void video_clear() {
	int i;
	
	for(i = 0; i < _VIDEO_SIZE; i++) {
		video[i].character = ' ';
		video[i].style = _VIDEO_STYLE_DEFAULT;
	}

	video_cursor_put(0);
}

void video_cursor_show(int boolean) {
	if(boolean) {
		cursor_shown = TRUE;
		video_cursor_put(cursor);
	} else {
		cursor_shown = FALSE;
		video_cursor_setter(_VIDEO_SIZE);
	}
}

int video_cursor_put(unsigned int position) {
	if(position >= _VIDEO_SIZE) {
		return _VIDEO_ERROR_CURSOR_INVALID;
	}
	
	cursor = position;

	if(cursor_shown) {
		video_cursor_setter(cursor);
	}

	return OK;
}

int video_cursor_get() {
	return cursor;
}

int video_cursor_shape(cursor_st shape) {
	uint8_t start, end;

	switch(shape) {
		case _VIDEO_CURSOR_SQUARE:
			start = CURSOR_SHAPE_SQUARE_START;
			end = CURSOR_SHAPE_SQUARE_END;
			break;
		case _VIDEO_CURSOR_UNDERSCORE:
			start = CURSOR_SHAPE_UNDERSCORE_START;
			end = CURSOR_SHAPE_UNDERSCORE_END;
			break;
		case _VIDEO_CURSOR_DOT:
			start = CURSOR_SHAPE_DOT_START;
			end = CURSOR_SHAPE_DOT_END;
			break;
		default:
			return _VIDEO_ERROR_CURSOR_SHAPE_INVALID;
	}

	video_cursor_shape_changer(start, end);

	return OK;
}

int video_cursor_shape_isValid(cursor_st shape) {
	switch(shape) {
		case _VIDEO_CURSOR_SQUARE:
		case _VIDEO_CURSOR_UNDERSCORE:
		case _VIDEO_CURSOR_DOT:
			return TRUE;
		default:
			return FALSE;
	}
}

int video_write(unsigned int position, char character) {
	if(position >= _VIDEO_SIZE) {
		return _VIDEO_ERROR_CURSOR_INVALID;
	}

	video[position].character = character;

	return OK;
}

int video_writeWithStyle(unsigned int position, char character, style_st style) {
	if(position >= _VIDEO_SIZE) {
		return _VIDEO_ERROR_CURSOR_INVALID;
	}

	video[position].character = character;
	video[position].style = style;

	return OK;
}

char video_get(unsigned int position) {
	if(position >= _VIDEO_SIZE) {
		return _VIDEO_ERROR_CURSOR_INVALID;
	}

	return video[position].character;
}

int video_style_put(unsigned int position, style_st style) {
	if(position >= _VIDEO_SIZE) {
		return _VIDEO_ERROR_CURSOR_INVALID;
	}

	video[position].style = style;

	return OK;
}

int video_style_get(unsigned int position) {
	if(position >= _VIDEO_SIZE) {
		return _VIDEO_ERROR_CURSOR_INVALID;
	}

	return video[position].style;
}

void video_style_all(style_st style) {
	video_style_range(0, _VIDEO_SIZE - 1, style);
}

int video_style_range(unsigned int from, unsigned int to, style_st style) {
	unsigned int i;

	if(from > to || from >= _VIDEO_SIZE || to >= _VIDEO_SIZE) {
		return _VIDEO_ERROR_RANGE_INVALID;
	}
	
	for(i = from; i <= to; i++) {
		video[i].style = style;
	}

	return OK;
}

int video_color_put(unsigned int position, style_st color) {
	if(position >= _VIDEO_SIZE) {
		return _VIDEO_ERROR_CURSOR_INVALID;
	}

	style_st style = video_style_get(position);
	video_style_put(position, _VIDEO_STYLER_COLOR(style, color));

	return OK;
}

int video_color_get(unsigned int position) {
	if(position >= _VIDEO_SIZE) {
		return _VIDEO_ERROR_CURSOR_INVALID;
	}

	return _VIDEO_COLOR_GETTER(video_style_get(position));
}

void video_color_all(style_st color) {
	video_color_range(0, _VIDEO_SIZE -1, color);
}

int video_color_range(unsigned int from, unsigned int to, style_st color) {
	unsigned int i;

	if(from > to || from >= _VIDEO_SIZE || to >= _VIDEO_SIZE) {
		return _VIDEO_ERROR_RANGE_INVALID;
	}
	
	for(i = from; i <= to; i++) {
		video[i].style = _VIDEO_STYLER_COLOR(video[i].style, color);
	}

	return OK;
}

int video_bg_put(unsigned int position, style_st bg) {
	if(position >= _VIDEO_SIZE) {
		return _VIDEO_ERROR_CURSOR_INVALID;
	}

	style_st style = video_style_get(position);
	video_style_put(position, _VIDEO_STYLER_BG(style, bg));

	return OK;
}

int video_bg_get(unsigned int position) {
	if(position >= _VIDEO_SIZE) {
		return _VIDEO_ERROR_CURSOR_INVALID;
	}

	return _VIDEO_BG_TO_COLOR(video_style_get(position));
}

void video_bg_all(style_st bg) {
	video_bg_range(0, _VIDEO_SIZE - 1, bg);
}

int video_bg_range(unsigned int from, unsigned int to, style_st bg) {
	unsigned int i;

	if(from > to || from >= _VIDEO_SIZE || to >= _VIDEO_SIZE) {
		return _VIDEO_ERROR_RANGE_INVALID;
	}
	
	for(i = from; i <= to; i++) {
		video[i].style = _VIDEO_STYLER_BG(video[i].style, bg);
	}

	return OK;
}

static void video_cursor_setter(unsigned int position) {
	_port_write_byte(PORT_VIDEO_INDEX, INDEX_CURSOR_LOCATION_HIGH_REGISTER);
	_port_write_byte(PORT_VIDEO_DATA, WORD_FIRST_HALF(position)); // High part
	_port_write_byte(PORT_VIDEO_INDEX, INDEX_CURSOR_LOCATION_LOW_REGISTER);
	_port_write_byte(PORT_VIDEO_DATA, WORD_LAST_HALF(position)); // Low part
}

static void video_cursor_shape_changer(uint8_t start, uint8_t end) {
	// -------------------------------------------------------------------------------
	// If Cursor Scan Line End < Cursor Scan Line Start, cursor will not be displayed!
	// -------------------------------------------------------------------------------
	_port_write_byte(PORT_VIDEO_INDEX, INDEX_CURSOR_START_REGISTER);
	_port_write_byte(PORT_VIDEO_DATA, GET_LAST_5BITS(start)); // Top most line
	_port_write_byte(PORT_VIDEO_INDEX, INDEX_CURSOR_END_REGISTER);
	_port_write_byte(PORT_VIDEO_DATA, GET_LAST_5BITS(end)); // Bottom most line
}
