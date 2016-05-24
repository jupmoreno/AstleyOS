#include <define.h>
#include <syscalls.h>
#include <sysio.h>
#include <sysconsole.h>
#include <systime.h>

/**
 * Syscall caller or wrapper
 * Makes the real assembly int 80h call
 * @param  rdi		|	
 * @param  rsi		| |	
 * @param  rdx 		| | | Syscalls 
 * @param  r10 		| | | Parameters
 * @param  r8  		| |	
 * @param  r9  		|
 * @param  rax		the syscall # to execute
 * @return     
 */
uint64_t _syscall(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9, uint64_t rax);

unsigned int sysread(char * buffer, unsigned int length) {
	return (unsigned int) _syscall(_FILEDESCRIPTOR_STDIN, (uint64_t) buffer, length, 0, 0, 0, _SYSCALL_READ);
}

unsigned int syswrite(char * string, unsigned int length) {
	return (unsigned int) _syscall(_FILEDESCRIPTOR_STDOUT, (uint64_t) string, length, 0, 0, 0, _SYSCALL_WRITE);
}

int systime(int operation, time_st * time) {
	return (int) _syscall(operation, (uint64_t) time, 0, 0, 0, 0, _SYSCALL_TIME);
}

int systerminal_select(int index) {
	return (int) _syscall(index, 0, 0, 0, 0, 0, _SYSCALL_TERMINAL_SELECT);
}

void systerminal_clear() {
	_syscall(0, 0, 0, 0, 0, 0, _SYSCALL_TERMINAL_CLEAR);
}

void systerminal_color(int operation, style_st color) {
	_syscall(operation, (uint64_t) color, 0, 0, 0, 0, _SYSCALL_TERMINAL_COLOR);
}

int systerminal_cursor(cursor_st cursor) {
	return (int) _syscall((uint64_t) cursor, 0, 0, 0, 0, 0, _SYSCALL_TERMINAL_CURSOR);
}
