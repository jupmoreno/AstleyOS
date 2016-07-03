#include "snake.h"
#include "libDraw.h"
#include "alloc.h"
#include <stdio.h>
#include <ipc.h>
#include "strings.h"
#include <songs.h>

extern int sys_getpid(void);
extern int sys_has_message(uint64_t pid);
extern read_msg sys_read_message(uint64_t pid);
extern int sys_rand(int first, int last);
extern int sys_new_process(const char* name, uint64_t func, uint64_t argc, void* argv);

locker_state board[BOARD_FILS][BOARD_COLS];
Snake_t snake = NULL;
game_state gameState = PLAYING;
direction actualDirection = RIGHT;
int blankLockers = BOARD_FILS * BOARD_COLS;
int eating = FALSE;

int score = 0;

void paint_locker(int fil, int col, color c){
	point p = toPoint(SQUARE_LENGTH * col, (SQUARE_LENGTH * fil) + 64);
	draw_frect(p, SQUARE_LENGTH, SQUARE_LENGTH, c);
}


void startGame(){
	
	prevGame();
	setGameFrame();
	snakeInit();
	addSnakeNode(10,10);
	addSnakeNode(10,11);
	addSnakeNode(10,12);

	printSnake();
	uint64_t pid = sys_getpid();
	newFruit();
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
			free(mensj);
		}
		
		milisecSleep(250);
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
		blankLockers--;
		return;
	}
	snake->head->next = node;
	snake->head = node;
	board[fil][col] = SNAKE;
	blankLockers--;
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
	blankLockers++;
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

void printFood(int fil, int col){
	int x = (SQUARE_LENGTH * col) + 16;
	int y = (SQUARE_LENGTH * fil) + 64 +16;
	draw_fcircle(toPoint(x,y), 10, orange);
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
	if(board[newFil][newCol] == SNAKE){
		gameState = GAME_OVER;
		return;
	}

	int gonnaEat = FALSE;
	if(board[newFil][newCol] == FOOD){
		eating = TRUE;
		gonnaEat = TRUE;
		raiseScore();
	}

	addSnakeNode(newFil, newCol);
	paint_locker(newFil, newCol, limegreen);

	if(gonnaEat){	
		newFruit();
	}
}

void newFruit(){
	point fruitPoint = getFruit();
	board[fruitPoint.y][fruitPoint.x] = FOOD;
	printFood(fruitPoint.y, fruitPoint.x);
}

point getFruit(){
	int rand = sys_rand(0, blankLockers);
	int i,j;
	int fruitFil, fruitCol;
	point p;

	for(i = 0; i < BOARD_FILS && rand >= 0; i++){
		for(j = 0; j < BOARD_COLS && rand >= 0; j++){
			if(board[i][j] == NOTHING){
				if(rand == 0)
					p = toPoint(j,i);
				rand--;
			}
		}
	}
	
	return p;
}

void raiseScore(){
	printScore(black);
	score += 100;
	printScore(red);
}

void setGameFrame(){
	draw_line(toPoint(0,64-1), toPoint(1024-1,64-1), white);
	draw_text("score:",6, toPoint(10,25), 3, cyan);
	printScore(cyan);
}

void printScore(color c){
	char string[10];
	char * s = strnum(score,10, string);
	draw_text(s,strlen(s), toPoint(275,25), 3, c);
}

void prevGame(){
		draw_text("WELCOME TO: ",13, toPoint(300,100), 2, red);
		draw_text("SUPER    EPIC    AWESOME",24, toPoint(130,250), 2, red);
		draw_text("RICK    ASTLEY    ADVENTURE:",28, toPoint(130,400), 2, red);
		draw_text("RICK ASNAKE",12, toPoint(300,550), 2, red);
		milisecSleep(1000);
		clear_screen();
}
