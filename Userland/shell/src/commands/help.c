#include <commands.h>
#include <stdio.h>
#include <strings.h>

COMMAND_FUNCTION(help);
COMMAND_HELP(help);

COMMAND_FUNCTION(help) {
	int i;

	if(args.argc == 0) {
		printf("Available commands:\n");
		for(i = 0; commands[i+1].name != NULL; i++) {
			printf("%s\t", commands[i].name);
		}
		printf("%s\n", commands[i].name);

		return OK;
	}

	for(i = 0; commands[i].name != NULL; i++) {
		if(!strcmp(args.argv[0], commands[i].name)) {
			ARGS_FOR_SUBCOMMAND(args);
			return commands[i].help(args);
		}
	}

	printf("<help> Error: invalid command.\n");
	return ERROR;
}

COMMAND_HELP(help) {
	if(args.argc != 0) {
		printf("<help> Error: too many arguments.\n");
		return ERROR;
	}

	printf("Displays the command's help. Use 'help' for list of available commands.\n");
	printf("Usage: 'help [command]'\n");

	return OK;
}
