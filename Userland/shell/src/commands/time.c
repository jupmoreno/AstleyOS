#include <commands.h>
#include <stdio.h>
#include <strings.h>
#include <numbers.h>
#include <time.h>

COMMAND_FUNCTION(time);
COMMAND_HELP(time);

SUBCOMMAND_FUNCTION(set);
COMMAND_HELP(set);

SUBCOMMAND_FUNCTION(get);
COMMAND_HELP(get);

#define SUBCOMMANDS_SIZE 2
command_t subcommands[SUBCOMMANDS_SIZE] = {
	SUBCOMMAND(set),
	SUBCOMMAND(get),
	// FUTURE SUBCOMMAND HERE !! REMEMBER TO CHANGE SUBCOMMANDS_SIZE !!
}; // TODO: NULL terminated?

COMMAND_FUNCTION(time) {
	int i;

	if(args.argc == 0) {
		ARGS_FOR_SUBCOMMAND(args);
		return SUBCOMMAND_RUN(get, args);
	}

	for(i = 0; i < SUBCOMMANDS_SIZE; i++) {
		if(!strcmp(args.argv[0], subcommands[i].name)) {
			ARGS_FOR_SUBCOMMAND(args);
			return subcommands[i].run(args);
		}
	}

	printf("<time> Error: invalid option.\n");
	return ERROR;
}

COMMAND_HELP(time) {
	int i;

	if(args.argc == 0) {
		printf("System time set/get. Use 'time' to display system time.\n");
		printf("Usage: 'time [operation] [arguments]'\n");
		return OK;
	}

	for(i = 0; i < SUBCOMMANDS_SIZE; i++) {
		if(!strcmp(args.argv[0], subcommands[i].name)) {
			ARGS_FOR_SUBCOMMAND(args);
			return subcommands[i].help(args);
		}
	}

	return OK;
}

SUBCOMMAND_FUNCTION(set) {
	time_st t;

	if(args.argc != 6) {
		printf("<time:set> Error: invalid number of arguments.\n");
		return ERROR;
	}

	t.day = numstr(args.argv[0]);
	t.month = numstr(args.argv[1]);
	t.year = numstr(args.argv[2]);
	t.hour = numstr(args.argv[3]);
	t.minute = numstr(args.argv[4]);
	t.second = numstr(args.argv[5]);

	printf("Setting time to: %d/%d/%d %d:%d:%d\n", 	t.day, t.month, t.year, t.hour, t.minute, t.second);
	if(ERROR_OCCURRED(stime(&t))) {
		printf("<time:set> Error: can't set time.\n");
	}
	
	return OK;
}

COMMAND_HELP(set) {
	if(args.argc >= 1) {
		printf("<echo> Error: too many arguments.\n");

		return ERROR;
	}

	printf("Change system time.\n");
	printf("Usage: 'time set <day> <month> <year> <hours> <minutes> <seconds>'\n");

	return OK;
}

SUBCOMMAND_FUNCTION(get) {
	time_st t;

	if(args.argc != 0) {
		printf("<time:get> Error: too many arguments.\n");
		return ERROR;
	}

	if(ERROR_OCCURRED(gtime(&t))) {
		printf("<time:get> Error: can't get time.\n");
		return ERROR;
	}

	printf("%d/%d/%d %d:%d:%d\n", 	t.day, t.month, t.year, t.hour, t.minute, t.second);

	return OK;
}

COMMAND_HELP(get) {
	if(args.argc >= 1) {
		printf("<echo> Error: too many arguments.\n");

		return ERROR;
	}

	printf("Display system time.\n");
	printf("Usage: 'time get'\n");

	return OK;
}
