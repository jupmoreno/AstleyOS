#include <scheduler.h>
#include <process.h>

static Scheduler scheduler;

void schedulerInit(){

	scheduler -> waitingpq = queueInit();
	scheduler -> blockedpq = queueInit();
}

void schedule(){

	SchedulerLL -> current = SchedulerLL -> current -> next;
}

SchedulerLL queueInit(){
	SchedulerLL q = kmalloc(sizeof(struct schedulerLL));
	q -> size = 0;
	q -> current = NULL;
	return q;
}

int addProcess(Process p, SchedulerLL q){
	if(p == NULL)
		return ERROR;
	if(q == NULL)
		return ERROR;
	LLnode node = kmalloc(sizeof(struct llnode));
	if(LLnode == NULL)
		return ERROR;
	if(q->size == 0){
		q->current = node;
		node -> next = node;
		node -> prev = node;
		q->size++;
		return SUCCESS;
	}
	q->current->prev ->next = node;
	node -> prev = q -> current -> prev;
	node -> next = q -> current;
	q -> current -> prev = node;
	q->size++;
	return SUCCESS; 
}

Process removeProcess(int pid, SchedulerLL q){
	int found = 0;
	LLnode node = q -> current;
	Process p = NULL;
	if(q->size < 1){
		return ERROR;
	}
	do{
		if(node->process->pid = pid)
			found = 1;
	}while(node != q ->current && !found);

	if(size == 1 && found){
		p = node -> process; 
		size --;
		q -> current = NULL;
		return p;
	}
	if(found){
		p = node->process;
		node -> prev -> next = node -> next;
		node -> next -> prev = node -> prev;
	}
	return ERROR;
}