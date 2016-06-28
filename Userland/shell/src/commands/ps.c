#include <commands.h>
#include <stdio.h>
#include <console.h>

extern void sys_ps(void);
extern void sys_new_process(const char* name, uint64_t func, uint64_t argc, void* argv);
void call_ps();

COMMAND_FUNCTION(ps);
COMMAND_HELP(ps);

COMMAND_FUNCTION(ps) {
	sys_new_process("ps", (uint64_t)&call_ps, 0, 0);
	return OK;
}

COMMAND_HELP(ps) {
	if(args.argc >= 1) {
		printf("<ps> Error: too many arguments.\n");

		return ERROR;
	}

	printf("Prints all the processes and it's attributes. Ignores arguments.\n");
	printf("Usage: 'ps'\n");

	return OK;
}

void call_ps(){
	sys_ps();
}


