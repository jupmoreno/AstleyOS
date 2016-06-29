#include <commands.h>
#include <stdio.h>
#include <console.h>
#include <numbers.h>

extern void sys_kill_process(int pid);
extern void sys_new_process(const char* name, uint64_t func, uint64_t argc, void* argv);
void call_kill(int argc, char * argv[]);

COMMAND_FUNCTION(kill);
COMMAND_HELP(kill);

COMMAND_FUNCTION(kill) {

	int pid = numstr(argv[0]);
	sys_kill_process(pid);

	return OK;
}

COMMAND_HELP(kill) {
	if(argc >= 2) {
		printf("<kill> Error: too many arguments.\n");

		return ERROR;
	}

	printf("Kills the process.\n");
	printf("Usage: 'kill [pid]'\n");

	return OK;
}
