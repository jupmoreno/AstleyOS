#include "snake.h"
#include "libDraw.h"
#include "alloc.h"

locker_state board[BOARD_FILS][BOARD_COLS];
Snake_t snake = NULL;


void paint_locker(int fil, int col, color c){
	point p = toPoint(SQUARE_LENGTH * col, (SQUARE_LENGTH * fil) + 64);
	draw_frect(p, SQUARE_LENGTH, SQUARE_LENGTH, c);
}

void snakeInit(){
	Snake_t ret = malloc(sizeof(struct snake_t));
	if(ret == NULL)
		return;
	ret->head = NULL;
	ret->tail = NULL;
	snake = ret;
}

void addSnakeNode(int fil, int col){
	if(snake == NULL)
		return;

	SnakeListNode node = malloc(sizeof(struct snakeListNode));

	if(node == NULL)
		return;

	node->fil = fil;
	node->col = col;
	node->next = NULL;

	if(snake->head == NULL){
		snake->head = node;
		snake->tail = node;
		board[fil][col] = SNAKE;
		return;
	}
	snake->head->next = node;
	snake->head = node;
	board[fil][col] = SNAKE;
	return;
}

SnakeListNode removeSnakeNode(){
	if(snake == NULL)
		return NULL;
	if(snake->tail == NULL){
		return NULL;
	}
	SnakeListNode ret = snake->tail;
	snake->tail = snake->tail->next;
	if(snake->tail == NULL){
		snake->head = NULL;
	}
	board[ret->fil][ret->col] = NOTHING;
	return ret;
}

void printSnake(){
	if(snake == NULL)
		return;
	SnakeListNode node = snake->tail;
	while(node != NULL){
		paint_locker(node->fil, node->col, limegreen);
		node = node->next;
	}
}