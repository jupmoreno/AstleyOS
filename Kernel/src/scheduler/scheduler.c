#include <scheduler.h>

static Scheduler scheduler;

void schedulerInit(){

	scheduler -> waitingpq = queueInit();
	scheduler -> blockedpq = queueInit();
}

void schedule(){

	scheduler -> waitingpq -> current = scheduler -> waitingpq-> current -> next;
}

SchedulerLL queueInit(){
	SchedulerLL q = kmalloc(sizeof(struct schedulerLL));
	q -> size = 0;
	q -> current = NULL;
	return q;
}

void printProcesses(){
	LLnode first = kmalloc(sizeof(struct llnode));
	LLnode current = kmalloc(sizeof(struct llnode));
	first = scheduler->waitingpq->current;
	current = scheduler->waitingpq->current;
	
	out_printf("Processes that are waiting:\n");
	out_printf("%s: pid %d", current->process->name, current->process->pid);
	while(current != first){
		current = current -> next;
		out_printf("%s: pid %d", current->process->name, current->process->pid);
	}
	
	current = scheduler->blockedpq->current;
	first = scheduler->blockedpq->current;
	
	out_printf("Processes that are blocked:\n");
	out_printf("%s: pid %d", current->process->name, current->process->pid);
	while(current != first){
		current = current -> next;
		out_printf("%s: pid %d", current->process->name, current->process->pid);
	}
	
}

int addProcess(Process p, SchedulerLL q){
	if(p == NULL)
		return ERROR;
	if(q == NULL)
		return ERROR;
	LLnode node = kmalloc(sizeof(struct llnode));
	if(node == NULL)
		return 0; //error
	if(q->size == 0){
		q->current = node;
		node -> next = node;
		node -> prev = node;
		q->size++;
		return 1;
	}
	q->current->prev ->next = node;
	node -> prev = q -> current -> prev;
	node -> next = q -> current;
	q -> current -> prev = node;
	q->size++;
	return 1; 
}

Process removeProcess(uint64_t pid, SchedulerLL q){
	int found = 0;
	LLnode node = q -> current;
	Process p = NULL;
	if(q->size < 1){
		return 0;
	}
	do{
		if(node->process->pid == pid)
			found = 1;
	}while(node != q ->current && !found);

	if(q->size == 1 && found){
		p = node -> process; 
		q->size --;
		q -> current = NULL;
		return p;
	}
	if(found){
		p = node->process;
		node -> prev -> next = node -> next;
		node -> next -> prev = node -> prev;
	}
	return 0;
}

Process getCurrentWaiting(){
	return scheduler -> waitingpq -> current -> process;
}

Process getCurrentBlocked(){
	return scheduler -> blockedpq -> current -> process;
}