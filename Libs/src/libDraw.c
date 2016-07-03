
/* 
 * libDraw library source file
 * libDraw is a library for BareMetal-OS to draw things.
 * Written by ohnx.
 *
 * https://github.com/ohnx/libDraw
 *
 */
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include "libBareMetal.h"
//#include <time.h>
#include "libDraw.h"
//#include <stdarg.h>
#include "font.h"
#include "numbers.h"

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

void clear_screen(){
	int i,j;
	for(i = 0; i < WIDTH; i++){
		for(j = 0; j < 768; j++){
			put_pixel(i,j,black);
		}
	}
}

void draw_line(point s, point e, color c) {
    /*if(s.x=e.x) {
      int i=0;
      int d = abs(s.y-e.y);
      for(; i<d; i++) {
	put_pixel(s.x,s.y+i,c);
      }
      return;
    }
    if(s.y=e.y) {
      int i=0;
      int d = abs(s.x-e.x);
      for(; i<d; i++) {
	put_pixel(s.x+i,s.y,c);
      }
      return;
    }*/
    int x = s.x;
    int y = s.y;
    int x2 = e.x;
    int y2 = e.y;
    int w = x2 - x;
    int h = y2 - y;
    int dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0, i = 0;
    if (w<0) dx1 = -1; else if (w>0) dx1 = 1;
    if (h<0) dy1 = -1; else if (h>0) dy1 = 1;
    if (w<0) dx2 = -1; else if (w>0) dx2 = 1;
    int longest = abs(w);
    int shortest = abs(h);
    if (!(longest>shortest)) {
        longest = abs(h);
        shortest = abs(w);
        if (h<0) dy2 = -1; else if (h>0) dy2 = 1;
        dx2 = 0 ;
    }
    int numerator = longest >> 1;
    for (i=0;i<=longest;i++) {
	put_pixel(x,y,c);
        numerator += shortest;
        if (!(numerator<longest)) {
            numerator -= longest;
            x += dx1;
            y += dy1;
        } else {
            x += dx2;
            y += dy2;
        }
    }
}

void draw_frect(point p, unsigned int l, unsigned int w, color c){
	unsigned int i, j;
	for(i=0; i<l; i++)
	{
		for(j=0; j<w; j++)
		{
			put_pixel(p.x+i, p.y+j, c);
		}
	}
}

//Found from http://stackoverflow.com/a/1237519
//Adapted for BareMetal-OS by ohnx
void draw_fcircle(point center, int radius, color c){
	int x, y;
	for(y=-radius; y<=radius; y++)
	    for(x=-radius; x<=radius; x++)
	        if(x*x+y*y <= radius*radius)
	            put_pixel(center.x+x, center.y+y, c);
}