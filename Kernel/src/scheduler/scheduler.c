#include "process.h"
#include <strings.h>

static uint64_t pids = 0;

uint64_t rsp;
uint64_t ss;
uint64_t reserved_pages;
uint64_t stack;


void create_process(char* name, process_func func, uint64_t argc, void* argv){
	Process *p;
	p = buddy_alloc(sizeof(Process));
	strcpy(p->name, name);
	p->pid =  pids++;
	p->state = WAITING;
	
	uint64_t* rsp = buddy_alloc(sizeof(stack_frame));
	p->stack = rsp;
	p->rsp = set_stack_frame(rsp, func, arc, argv);
}


uint64_t set_stack_frame(uint64_t *rsp, process_func func, uint64_t argc, void * argv){

	stack_frame *r = (stack_frame*) (rsp);
		
	r->gs = 0x001;
	r->fs = 0x002;
	r->r15= 0x003;
	r->r14= 0x004;
	r->r13= 0x005;
	r->r12= 0x006;
	r->r11= 0x007;
	r->r10= 0x008;
	r->r9= 0x009;
	r->r8= 0x00A;
	r->rsi= argc;
	r->rdi= func;
	r->rbp= 0x00D;
	r->rdx= argv;
	r->rcx= 0x00F;
	r->rbx= 0x010;
	r->rax= 0x011;
	
	//iretq hook
	
	r->rip= &start;
	r->cs= 0x008;
	r->eflags= 0x202;
	r->rsp= (uint64_t)&(r->base);
	r->ss= 0x000;
	
	r->base= 0x000;
	
	return &(r->gs);
}




	
	