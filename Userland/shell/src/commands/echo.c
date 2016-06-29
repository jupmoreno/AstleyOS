#include <commands.h>
#include <stdio.h>

COMMAND_FUNCTION(echo);
COMMAND_HELP(echo);

COMMAND_FUNCTION(echo) {
	int i;

	for(i = 0; i < argc; i++) {
		printf("%s ", argv[i]);
	}
	printf("\n");

	return OK;
}

COMMAND_HELP(echo) {
	if(argc >= 1) {
		printf("<echo> Error: too many arguments.\n");

		return ERROR;
	}

	printf("Write arguments to the standard output.\n");
	printf("Usage: 'echo [string ...]'\n");

	return OK;
}
