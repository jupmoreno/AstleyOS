#ifndef _KSYSCALLS_H_
#define _KSYSCALLS_H_

#include <kdefine.h>
#include <syscalls.h>

typedef uint64_t syscall_st(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9);

syscall_st syscall_read;
syscall_st syscall_write;
syscall_st syscall_time;
syscall_st syscall_terminal_select;
syscall_st syscall_terminal_clear;
syscall_st syscall_terminal_color;
syscall_st syscall_terminal_cursor;
syscall_st syscall_set_video_mode;
syscall_st syscall_paint_pixel;
syscall_st syscall_sound;
syscall_st syscall_new_process;
syscall_st syscall_ps;
syscall_st syscall_kill_process;
syscall_st syscall_block_process;
syscall_st syscall_waitpid;
syscall_st syscall_get_pid;
syscall_st syscall_alloc;
// (2) FUTURE SYSCALL HERE !!

extern syscall_st * syscalls_table[_SYSCALLS_SIZE];

#endif
