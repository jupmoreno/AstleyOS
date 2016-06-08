#include <ksyscalls.h>
#include <manager.h>
#include <sysio.h>
#include <sysconsole.h>

extern unsigned int manage_read(unsigned int fd, char * buffer, unsigned int length);
extern unsigned int manage_write(unsigned int fd, const char * string, unsigned int length);
extern int manage_terminal(int operation, int value);
extern void * manage_alloc(int op, ...);

syscall_st * syscalls_table[_SYSCALLS_SIZE] = {
	[_SYSCALL_READ] = syscall_read,
	[_SYSCALL_WRITE] = syscall_write,
	[_SYSCALL_TERMINAL_SELECT] = syscall_terminal_select,
	[_SYSCALL_TERMINAL_CLEAR] = syscall_terminal_clear,
	[_SYSCALL_TERMINAL_COLOR] = syscall_terminal_color,
	[_SYSCALL_TERMINAL_CURSOR] = syscall_terminal_cursor,
	[_SYSCALL_ALLOC] = syscall_alloc,
	// (3) FUTURE SYSCALL HERE !! REMEMBER TO CHANGE _SYSCALLS_LAST !!
};

uint64_t syscall_read(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
	int fd = (int) rdi;
	char * buffer = (char *) rsi;
	unsigned int count = (unsigned int) rdx;

	return manage_read(fd, buffer, count);
}

uint64_t syscall_write(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
	int fd = (unsigned int) rdi;
	const char * string = (const char *) rsi;
	unsigned int count = (unsigned int) rdx;

	return manage_write(fd, string, count);
}

uint64_t syscall_terminal_select(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
	int index = (int) rdi;

	return manage_terminal(_TERMINAL_OPERATION_SELECT, index);
}

uint64_t syscall_terminal_clear(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
	return manage_terminal(_TERMINAL_OPERATION_CLEAR, 0);
}

uint64_t syscall_terminal_color(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
	int operation = (int) rdi;
	int color = (int) rsi;

	return manage_terminal(operation, color);
}

uint64_t syscall_terminal_cursor(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
	int cursor = (int) rdi;

	return manage_terminal(_TERMINAL_OPERATION_CURSOR, cursor);
}

uint64_t syscall_alloc(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
	int op = (int) rdi;

	return (uint64_t) manage_alloc(op, rsi, rdx);
}
