#include <modules.h>
#include <memory.h>
#include <output.h>
#include <pit.h>
#include <serial.h>
#include <sound.h>
#include <kalloc.h>
#include <heap.h>
#include <process.h>
#include <scheduler.h>
#include <messageManager.h>

static blockedNode blist;
extern void _interrupt_20();

 void block(uint64_t id){
 	blockedNode node = kmalloc(sizeof (struct blockedN));
 	node -> id = id;
 	if(blist == NULL){
 		blist = node;
 		node -> next = NULL;
 		return;
 	}
 	node -> next = blist;
 	blist = node;
 	blockProcess(id);
	_interrupt_20();
 }

 void unblock(uint64_t id){
 	if(blist == NULL){
 		return;
 	}
 	blockedNode current = blist;
 	blockedNode prev = NULL;
 	while(current != NULL && current -> id != id){
 		prev = current;
 		current = current -> next;
 	}
 	if(current == blist){
 		unblockProcess(id);
 		blist = blist -> next; 
 	}
 	if(current == NULL){
 		return;
 	}
 	prev -> next = current -> next;
 	kfree(current);
 	unblockProcess(id);
 }

 char isMQBlocked(uint64_t id){
 	if(blist == NULL){
 		return 0;
 	}
 	blockedNode current = blist;
 	blockedNode prev = NULL;
 	while(current != NULL && current -> id != id){
 		prev = current;
 		current = current -> next;
 	}
 	if(current == NULL){
 		return 0;
 	}
 	return 1;
 }