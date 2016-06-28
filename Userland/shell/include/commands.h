#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include <define.h>

// Addapted from https://github.com/sebikul/UnicOS
#define COMMAND_FUNCTION(name) int cmd_ ## name(int argc, char **argv)
#define SUBCOMMAND_FUNCTION(name) static int scmd_ ## name(int argc, char **argv)
#define COMMAND_HELP(name) int help_ ## name(int argc, char **argv)

#define COMMAND(name) {#name, cmd_ ## name, help_ ## name}
#define SUBCOMMAND(name) {#name, scmd_ ## name, help_ ## name}

#define COMMAND_DECLARE(name) extern COMMAND_FUNCTION(name); extern COMMAND_HELP(name)
//#define COMMAND_RUN(name, args) cmd_ ## name(args)
//#define SUBCOMMAND_RUN(name, args) scmd_ ## name(args)

typedef struct {
	const char * name;
	int (*run)(int argc, char **argv);
	int (*help)(int argc, char **argv);
} command_t;

extern command_t commands[];

#endif
