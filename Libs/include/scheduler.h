 struct llnode{
 	struct llnode * next;
 	struct llnode * prev;
 	Process process;
 };

 struct schedulerLL{
 	struct llnode * current;
 	unint64_t size;	
 };

 struct scheduler{
 	struct schedulerLL * waitingpql;
 	struct schedulerLL * blockedpq;
 };

 typedef struct llnode * LLnode; 
 typedef struct schedulerLL * SchedulerLL;
 typedef struct scheduler * Scheduler;