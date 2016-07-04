#include <ipc.h>
#include <snake_input.h>
#include <stdio.h>

extern int sys_getpid(void);

extern read_msg sys_read_message(uint64_t pid);
extern void sys_send_message(uint64_t sender, uint64_t receiver, uint64_t size, void* message);
extern int sys_has_message(uint64_t pid);

void snake_input(){
	uint64_t mypid = sys_getpid();
	read_msg mensj = sys_read_message(mypid);
	uint64_t game_pid = (uint64_t) mensj->msg;
	free(mensj);
	char c;
	while(1){
		c = scanc();
		if(c == 'w'){
			sys_send_message(mypid, (uint64_t)game_pid, 1, (void*)"w");
		}else if(c == 'a'){
			sys_send_message(mypid, (uint64_t)game_pid, 1, (void*)"a");
		}else if(c == 's'){
			sys_send_message(mypid, (uint64_t)game_pid, 1, (void*)"s");
		}else if(c == 'd'){
			sys_send_message(mypid, (uint64_t)game_pid, 1, (void*)"d");
		}else if(c == '\n'){
			sys_send_message(mypid, (uint64_t)game_pid, 1, (void*)"\n");
		}
	}
}
