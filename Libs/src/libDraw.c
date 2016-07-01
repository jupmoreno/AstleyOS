
/* 
 * libDraw library source file
 * libDraw is a library for BareMetal-OS to draw things.
 * Written by ohnx.
 */
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include "libBareMetal.h"
//#include <time.h>
#include "libDraw.h"
//#include <stdarg.h>
#include "font.h"

extern void syspaint_pixel(int x, int y, char blue, char green, char red);

//Colors
color red = {255, 0, 0};
color orange = {255, 140, 0};
color yellow = {255, 255, 0};
color limegreen = {50, 205, 0};
color brightgreen = {0, 255, 0};
color blue = {0, 0, 255};
color dodgerblue = {30, 144, 255};
color indigo = {199, 21, 133};
color violet = {75, 0, 130};
color aqua = {0, 200, 200};
color black = {0, 0, 0};
color white = {255, 255, 255};
color brown = {139, 69, 19};
color cyan = {0, 139, 139};
color gold = {255, 215, 0};
color crimson = {220, 20, 60};
color pink = {255, 205, 180};
color purple = {153, 50, 204};

//Character spacing
int libDraw_text_character_spacing = 2;

//Conversion tool from rgb color to color
color toColor(unsigned char rred, unsigned char ggreen, unsigned char bblue){color t={rred,ggreen,bblue};return t;}
//Conversion tool from x, y to point
point toPoint(unsigned int x, unsigned int y){point t={x,y};return t;}

void put_pixel(unsigned int x, unsigned int y, color c){
	syspaint_pixel(x,y,c.blue,c.green,c.red);
}

void draw_point(point p, unsigned int thickness, color c)
{
	int i,j,thick;
	thick=thickness/2;
	for(i=-thick;i<thick;i++)
		for(j=-thick;j<thick;j++)
			put_pixel(p.x+i, p.y+j, c);
}

void draw_schar(char to, point where, color c)
{
	int *bitmap = font[(int)to];
	int x,y,set;
	for (y=12; y >= 0; y--) {
		for (x=7; x >= 0; x--) {
			set = bitmap[y] & 1 << x;
			if(set) put_pixel(where.x+(12-x), where.y+(7-y), c);
		}
	}
}

void draw_char(char to, point where, int size, color c)
{
	int *bitmap = font[(int)to];
	int x,y,set,xe,ye;
	for (y=12; y >= 0; y--) {
		for (x=7; x >= 0; x--) {
			set = bitmap[y] & 1 << x;
			if(set)
			  	for(xe=0;xe<size;xe++)
				  	for(ye=0;ye<size;ye++)
						put_pixel(where.x+size*(12-x)+xe, where.y+size*(7-y)+ye, c);
		}
	}
}

void draw_text(const char* text, int length, point start, int size, color c)
{
	int i=0;
	int len=length;
	if(size==1) {
	  	for(;i<len;i++) {
	  		draw_schar(*(text+i), toPoint(start.x+i*7+2, start.y), c);
		}
		return;
	}
	draw_char(*text, toPoint(start.x+i*7*size, start.y), size, c);
	for(i=1;i<len;i++) {
	  draw_char(*(text+i), toPoint(start.x+i*7*size*libDraw_text_character_spacing, start.y), size, c);
	}
}