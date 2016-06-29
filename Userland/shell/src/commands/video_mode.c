#include <commands.h>
#include <stdio.h>
#include <console.h>

COMMAND_FUNCTION(video_mode);
COMMAND_HELP(video_mode);

COMMAND_FUNCTION(video_mode) {
//	SetVideoMode();
	clear();
	return OK;
}

COMMAND_HELP(video_mode) {
	if(args.argc >= 1) {
		printf("<clear> Error: too many arguments.\n");

		return ERROR;
	}

	printf("Sets terminal in video mode. Ignores arguments.\n");

	return OK;
}
