/* 
 * libDraw library header file
 * libDraw is a library for BareMetal-OS to draw things.
 * Written by ohnx.
*/
#ifndef LIBDRAW_INC
#define LIBDRAW_INC
//Struct definitions (to avoid having like "unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int thickness, color c")
typedef struct {
	unsigned int x, y;
} point;
typedef struct {
	unsigned char red, green, blue;
} color; //Going with the American spelling for colour here

color toColor(unsigned char red, unsigned char green, unsigned char blue);
point toPoint(unsigned int x, unsigned int y);
/*int init_libDraw();
void clear_screen();
void clear_buffer();
void flush_buffer();
color get_pixel(unsigned int x, unsigned int y);
void draw_line(point s, point e, color c);
void draw_frect(point p, unsigned int l, unsigned int w, color c);
void draw_fcircle(point center, int radius, color c);
//Polygon function
void draw_polygon(color c, int points, ...);*/
//Text functions
void draw_schar(char to, point where, color c);
void draw_char(char to, point where, int size, color c);
void draw_text(const char* text, int length, point start, int size, color c);
//Keeping original put_pixel for coords because I'm lazy.
void put_pixel(unsigned int x, unsigned int y, color c);
void draw_point(point p, unsigned int thickness, color c);
void clear_screen();
//Variables
/*unsigned long VideoX, VideoY, VideoBPP, VideoMemSize;
char* VideoMemory;
char* VideoBuffer;*/

extern color red;
extern color orange;
extern color yellow;
extern color limegreen;
extern color brightgreen;
extern color blue;
extern color dodgerblue;
extern color indigo;
extern color violet;
extern color aqua;
extern color black;
extern color white;
extern color brown;
extern color cyan;
extern color gold;
extern color crimson;
extern color pink;
extern color purple;

extern int libDraw_text_character_spacing;

#define WIDTH 1024
#define HEIGHT 768

#endif