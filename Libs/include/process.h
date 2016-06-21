typedef struct process_queue{
	
	struct Process* head;
	struct Process* tail;
	
}process_queue;


typedef struct Process{
	uint64_t stack;
	uint64_t rsp;
	uint64_t reserved;
	
	
	char* name;
	unsigned int pid;
	
	process_t *next;
	process_t *prev;

	state_t state;

}process_t;

typedef enum state_t{RUNNING, WAITING, BLOCKED, DEAD} state_t;



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


typedef int (*process_func) (int argc, char *argv);