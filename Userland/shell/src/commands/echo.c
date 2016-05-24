#include <commands.h>
#include <stdio.h>

COMMAND_HELP(echo);

COMMAND_FUNCTION(echo) {
	unsigned int i;

	for(i = 0; i < args.argc; i++) {
		printf("%s ", args.argv[i]);
	}
	printf("\n");

	return OK;
}

COMMAND_HELP(echo) {
	if(args.argc >= 1) {
		printf("<echo> Error: too many arguments.\n");

		return ERROR;
	}

	printf("Write arguments to the standard output.\n");
	printf("Usage: 'echo [string ...]'\n");

	return OK;
}
