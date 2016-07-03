#ifndef _PROCESS_H_
#define _PROCESS_H_
#include <stdint.h>
#define STACK_SIZE 0x800000

typedef int (*process_func) (int argc, char **argv);
typedef enum state_t{RUNNING, BLOCKED, WAITING, DEAD}state_t;

static const char* states[DEAD + 1] = {"RUNNING", "BLOCKED", "WAITING", "DEAD"};

typedef struct process{
	const char* name;
	uint64_t reserved;
	uint64_t pid;
	uint64_t father;
	uint64_t stackF;
	uint64_t rsp;

	state_t state;

}process;

typedef struct process_queue{
	
	struct process* head;
	struct process* tail;
	
}process_queue;

typedef struct process* Process;

typedef struct stack_frame {
	uint64_t gs;
	uint64_t fs;
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;
	uint64_t rsi;
	uint64_t rdi;
	uint64_t rbp;
	uint64_t rdx;
	uint64_t rcx;
	uint64_t rbx;
	uint64_t rax;
	
	//iretq hook
	uint64_t rip;
	uint64_t cs;
	uint64_t eflags;
	uint64_t rsp;
	uint64_t ss;
	uint64_t base;

	//exit hook	
	
}stack_frame;


uint64_t contextSwitch(uint64_t stackFrame);
uint64_t create_process(const char* name, process_func func, uint64_t argc, char* argv[]);
int kwaitpid(int pid);
//void start();

#endif