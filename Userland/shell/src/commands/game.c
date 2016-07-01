#include <commands.h>
#include <stdio.h>
#include <console.h>

extern void sysvideo_mode(void);
extern void syspaint_pixel(int x, int y, int blue, int green, int red);
void paintPixel(int x, int y, int blue, int green, int red);
extern void sys_sound(int frequency, double time);

COMMAND_FUNCTION(game);
COMMAND_HELP(game);

COMMAND_FUNCTION(game) {
	sysvideo_mode();
	
	
	int i,j,c;
	i = 0;
	while(1){
		c = scanc();
		for(j=0;j<700;j++)
			paintPixel(i,j,j,i,200);
		i++;
	}
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

void paintPixel(int x, int y, int blue, int green, int red){
	syspaint_pixel(x,y,blue,green,red);	
}
