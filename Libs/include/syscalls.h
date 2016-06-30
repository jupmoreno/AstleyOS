#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <define.h>

enum syscall_triggers {
	_SYSCALL_READ = 0,
	_SYSCALL_WRITE,
	_SYSCALL_TERMINAL_SELECT,
	_SYSCALL_TERMINAL_CLEAR,
	_SYSCALL_TERMINAL_COLOR,
	_SYSCALL_TERMINAL_CURSOR,
	_SYSCALL_VIDEO_MODE,
	_SYSCALL_PAINT_PIXEL,
	_SYSCALL_SOUND,
	_SYSCALL_NEW_PROCESS,
	_SYSCALL_PS,
	_SYSCALL_KILL_PROCESS,
	_SYSCALL_BLOCK_PROCESS,
	_SYSCALL_WAITPID,
	_SYSCALL_GET_PID,
	_SYSCALL_ALLOC
	// (1) FUTURE SYSCALL HERE !! REMEMBER TO CHANGE _SYSCALLS_LAST !!
};

#define _SYSCALLS_LAST _SYSCALL_ALLOC
#define _SYSCALLS_SIZE (_SYSCALLS_LAST + 1)

#endif