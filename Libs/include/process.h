typedef struct process_queue{
	
	struct Process* head;
	struct Process* tail;
	
}process_queue;


typedef struct Process{
	
	char* name;
	unsigned int pid;
	
	struct Process *next;
	struct Process *prev;

	state_t state;

}Process;

typedef enum state_t{RUNNING, WAITING, BLOCKED, DEAD} state_t;