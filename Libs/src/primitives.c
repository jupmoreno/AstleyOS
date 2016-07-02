#include <syscalls.h>
#include <sysio.h>
#include <sysconsole.h>
#include <sysalloc.h>

#include <ipc.h> //TODO esto no deberia estar en kernel ojo revisar

unsigned int sysread(char * buffer, unsigned int length);
unsigned int syswrite(char * string, unsigned int length);
int systerminal_select(int index);
void systerminal_clear(void);
void systerminal_color(int operation, style_st color);
int systerminal_cursor(cursor_st cursor);
void * sysalloc_new(unsigned int size);
void sysalloc_free(void * addr);
void sysvideo_mode(void);
void syspaint_pixel(int x, int y, char blue, char green, char red);
void sys_sound(int frequency, double time);
int sys_new_process(const char* name, uint64_t func, uint64_t argc, char* argv[]);
void sys_kill_process(int pid);
int sys_block_process(int pid);
void sys_ps(void);
int sys_waitpid(int pid);
int sys_getpid(void);
read_msg sys_read_message(uint64_t pid);
void sys_send_message(uint64_t sender, uint64_t receiver, uint64_t size, void* message);
void sys_play_note(uint32_t frec, uint32_t length);
void sys_sleep(uint32_t seconds);


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

int systerminal_select(int index) {
	return (int) _syscall(index, 0, 0, 0, 0, 0, _SYSCALL_TERMINAL_SELECT);
}

void systerminal_clear(void) {
	_syscall(0, 0, 0, 0, 0, 0, _SYSCALL_TERMINAL_CLEAR);
}

void systerminal_color(int operation, style_st color) {
	_syscall(operation, (uint64_t) color, 0, 0, 0, 0, _SYSCALL_TERMINAL_COLOR);
}

int systerminal_cursor(cursor_st cursor) {
	return (int) _syscall((uint64_t) cursor, 0, 0, 0, 0, 0, _SYSCALL_TERMINAL_CURSOR);
}

void * sysalloc_new(unsigned int pages) {
	return (void *) _syscall(_ALLOC_NEW, pages, 0, 0, 0, 0, _SYSCALL_ALLOC);
}

void sysalloc_free(void * addr) {
	_syscall(_ALLOC_FREE, (uint64_t) addr, 0, 0, 0, 0, _SYSCALL_ALLOC);
}

void sysvideo_mode(void) {
	_syscall(0, 0, 0, 0, 0, 0, _SYSCALL_VIDEO_MODE);
}

void syspaint_pixel(int x, int y, char blue, char green, char red){
	_syscall((uint64_t)x,(uint64_t)y,(uint64_t)blue,(uint64_t)green,(uint64_t)red,0, _SYSCALL_PAINT_PIXEL);
}

void sys_sound(int frequency, double time) {
	_syscall((uint64_t)frequency,(uint64_t) time, 0, 0, 0, 0, _SYSCALL_SOUND);
}

int sys_new_process(const char* name, uint64_t func, uint64_t argc, char* argv[]) {
	return (int) _syscall((uint64_t)name, func, argc, (uint64_t) argv, 0, 0, _SYSCALL_NEW_PROCESS);
}

void sys_ps() {
	_syscall(0, 0, 0, 0, 0, 0, _SYSCALL_PS);
}

void sys_kill_process(int pid) {
	_syscall((uint64_t) pid, 0, 0, 0, 0, 0, _SYSCALL_KILL_PROCESS);
}

int sys_block_process(int pid) {
	return _syscall((uint64_t) pid, 0, 0, 0, 0, 0, _SYSCALL_BLOCK_PROCESS);
}

int sys_waitpid(int pid) {
	return _syscall((uint64_t) pid, 0, 0, 0, 0, 0, _SYSCALL_WAITPID);
}

int sys_getpid() {
	return _syscall(0, 0, 0, 0, 0, 0, _SYSCALL_GET_PID);
}

read_msg sys_read_message(uint64_t pid){
	return (read_msg)_syscall(pid, 0, 0, 0, 0, 0, _SYSCALL_READ_MESSAGE);
}

void sys_send_message(uint64_t sender, uint64_t receiver, uint64_t size, void* message){
	_syscall(sender, receiver, size, (uint64_t)message, 0, 0, _SYSCALL_SEND_MESSAGE);
}

void sys_play_note(uint32_t frec, uint32_t length){
	_syscall(frec, length, 0, 0, 0, 0, _SYSCALL_PLAY_NOTE);
}

// void * sysalloc_renew(void * addr, unsigned int size) {
// 	return (void *) _syscall(_ALLOC_RENEW, (uint64_t) addr, size, 0, 0, 0, _SYSCALL_ALLOC);
// }
void sys_sleep(uint32_t seconds){
	_syscall(seconds, 0, 0, 0, 0, 0, _SYSCALL_SLEEP);
}