#include <commands.h>
#include <stdio.h>
#include <console.h>

extern void sys_ps(void);
extern void sys_new_process(const char* name, uint64_t func, uint64_t argc, void* argv);

COMMAND_FUNCTION(ps);
COMMAND_HELP(ps);

COMMAND_FUNCTION(ps) {
	sys_ps();
	return OK;
}

COMMAND_HELP(ps) {
	if(argc >= 1) {
		printf("<ps> Error: too many arguments.\n");

		return ERROR;
	}

	printf("Prints all the processes and it's attributes. Ignores arguments.\n");
	printf("Usage: 'ps'\n");

	return OK;
}


