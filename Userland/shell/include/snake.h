#include "libDraw.h"
#include <define.h>

#define WIDTH 1024
#define HEIGHT 768
#define SQUARE_LENGTH 32
#define BOARD_COLS 32
#define BOARD_FILS 22

typedef enum locker_state{NOTHING, FOOD, SNAKE}locker_state;
typedef enum direction{UP, DOWN, RIGHT, LEFT}direction;
typedef enum game_state{PLAYING, GAME_OVER}game_state;

struct snakeListNode{
	struct snakeListNode * next;
	int fil;
	int col;
};

typedef struct snakeListNode * SnakeListNode; 

struct snake_t{
	struct snakeListNode * tail;
	struct snakeListNode * head;
};

typedef struct snake_t * Snake_t; 

void startGame(void);
void snakeInit(void);
void addSnakeNode(int fil, int col);
SnakeListNode removeSnakeNode();
void printSnake(void);
void moveSnake(void);
void printFood(int fil, int col);
point getFruit(void);
void newFruit(void);
void raiseScore(void);
void prevGame(void);
void freeSnake(void);
void cleanBoard(void);


void paint_locker(int fil, int col, color c);
void setGameFrame(void);
void printScore(color c);
void gameOverScreen(void);