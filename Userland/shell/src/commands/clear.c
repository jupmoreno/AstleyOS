#include <commands.h>
#include <stdio.h>
#include <console.h>

COMMAND_HELP(clear);

COMMAND_FUNCTION(clear) {
	clear();

	return OK;
}

COMMAND_HELP(clear) {
	if(args.argc >= 1) {
		printf("<clear> Error: too many arguments.\n");

		return ERROR;
	}

	printf("Clears the screen. Ignores arguments.\n");
	printf("Usage: 'clear'\n");

	return OK;
}
