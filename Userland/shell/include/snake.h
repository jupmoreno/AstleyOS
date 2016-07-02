#include "libDraw.h"

#define WIDTH 1024
#define HEIGHT 768
#define SQUARE_LENGTH 32
#define BOARD_COLS 32
#define BOARD_FILS 22

typedef enum locker_state{NOTHING, FOOD, SNAKE}locker_state;
typedef enum direction{UP, DOWN, RIGHT, LEFT}direction;


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

void snakeInit();
void addSnakeNode(int fil, int col);
SnakeListNode removeSnakeNode();
void printSnake();

void paint_locker(int fil, int col, color c);
