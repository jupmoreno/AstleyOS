#include <commands.h>
#include <stdio.h>
#include <console.h>
#include <libDraw.h>
#include <snake.h>

extern void sysvideo_mode(void);
extern void syspaint_pixel(int x, int y, int blue_c, int green_c, int red_c);
void paintPixel(int x, int y, int blue_c, int green_c, int red_c);
extern void sys_sound(int frequency, double time);

COMMAND_FUNCTION(game);
COMMAND_HELP(game);

COMMAND_FUNCTION(game) {
	sysvideo_mode();
	
	
	int i,j,c;
	i = 0;
	char d;
	draw_line(toPoint(0,64-1), toPoint(1024-1,64-1), yellow);
	paint_locker(0, 0, orange);
/*	draw_fcircle(toPoint(400,400), 300, pink);
	draw_fcircle(toPoint(300,200), 25, cyan);
	draw_fcircle(toPoint(500,200), 25, cyan);
	draw_frect(toPoint(300,450), 225, 20, cyan);
	while(1){
		c = scanc();
		clear_screen();
		draw_schar((char)c,toPoint(100,100),aqua);
		*//*for(j=0;j<700;j++)
			paintPixel(i,j,j,i,200);
		i++;
	}*/
	/*for(i=0; i<200; i++){
		for (j = 0; j < 200; j++)
		{
			paintPixel(i,j,100,i,200-j);
		}
	}*/
	while(1); // TODO: REMOVE
	//startGame();

	return OK;
}

COMMAND_HELP(game) {
	if(argc >= 1) {
		printf("<game> Error: too many arguments.\n");

		return ERROR;
	}

	printf("Runs the snake game. Ignores arguments.\n");
	printf("Usage: 'game'\n");

	return OK;
}

void paintPixel(int x, int y, int blue_c, int green_c, int red_c){
	syspaint_pixel(x,y,blue_c,green_c,red_c);	
}
