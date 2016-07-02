#include "snake.h"
#include "libDraw.h"

int board[BOARD_FILS][BOARD_COLS];


void paint_locker(int fil, int col, color c){
	point p = toPoint(SQUARE_LENGTH * col, (SQUARE_LENGTH * fil) + 64);
	draw_frect(p, SQUARE_LENGTH, SQUARE_LENGTH, c);
}