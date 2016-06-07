#ifndef _MANAGER_H_
#define _MANAGER_H_

#include <kdefine.h>
#include <sysconsole.h>

enum terminal_ops {
	_TERMINAL_OPERATION_SELECT = _TERMINAL_COLOR_OPERATION_STYLE + 1,
	_TERMINAL_OPERATION_CLEAR,
	_TERMINAL_OPERATION_CURSOR
};

#endif
