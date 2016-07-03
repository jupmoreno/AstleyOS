#include "snake.h"
#include "libDraw.h"
#include "alloc.h"
#include <songs.h>
#include <stdio.h>
#include <ipc.h>

extern void sys_sleep(uint32_t seconds);
extern int sys_getpid(void);
extern int sys_has_message(uint64_t pid);
extern read_msg sys_read_message(uint64_t pid);

locker_state board[BOARD_FILS][BOARD_COLS];
Snake_t snake = NULL;
game_state gameState = PLAYING;
direction actualDirection = RIGHT;

int eating = FALSE;


void paint_locker(int fil, int col, color c){
	point p = toPoint(SQUARE_LENGTH * col, (SQUARE_LENGTH * fil) + 64);
	draw_frect(p, SQUARE_LENGTH, SQUARE_LENGTH, c);
}


void startGame(){
	snakeInit();
	addSnakeNode(10,10);
	addSnakeNode(10,11);
	addSnakeNode(10,12);
	printSnake();
	uint64_t pid = sys_getpid();
	while(gameState == PLAYING){
		if(sys_has_message(pid)){
			read_msg mensj = sys_read_message(pid);
			char* cp = (char*)mensj->msg;
			char c = *cp;
			if(c == 'a'){
				if(actualDirection != RIGHT)
					actualDirection = LEFT;
			}else if(c == 's'){
				if(actualDirection != UP)
					actualDirection = DOWN;
			}else if(c == 'd'){
				if(actualDirection != LEFT)
					actualDirection = RIGHT;
			}else if(c == 'w'){
				if(actualDirection != DOWN)
					actualDirection = UP;
			}
		}
		
		sys_sleep(1);
		moveSnake();
	}

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

void moveSnake(){
	
	if(!eating){
		SnakeListNode node = removeSnakeNode();
		if(node == NULL)
			return;
		paint_locker(node->fil, node->col, black);
		free(node);
	}else{
		eating = FALSE;
	}

	int newFil = snake->head->fil;
	int newCol = snake->head->col;

	if(actualDirection == UP){
		newFil--;
	}else if(actualDirection == DOWN){
		newFil++;
	}else if(actualDirection == RIGHT){
		newCol++;
	}else{
		newCol--;
	}

	if(newCol < 0 || newCol >= BOARD_COLS || newFil < 0 || newFil >= BOARD_FILS){
		gameState = GAME_OVER;
		return;
	}
	if(board[newFil][newCol] != NOTHING){
		gameState = GAME_OVER;
		return;
	}

	addSnakeNode(newFil, newCol);
	paint_locker(newFil, newCol, limegreen);
}