#include "waitpidHistory.h"
#include "process.h"
#include <strings.h>
#include "kalloc.h"
#include <scheduler.h>
#include <output.h>

WaitpidHistory removeWaitpidHistoryRec(int child, WaitpidHistory actual);

WaitpidHistory wh = NULL;

void addWaitpidHistory(int father, int child){
	WaitpidHistory newWH = kmalloc(sizeof(struct waitpidHistory));
	newWH->father = (uint64_t) father;
	newWH->child = (uint64_t) child;
	newWH->next = wh;
	wh = newWH;
}

void removeWaitpidHistory(int child){
removeWaitpidHistoryRec(child, wh);
}

WaitpidHistory removeWaitpidHistoryRec(int child, WaitpidHistory actual){
	if(actual == NULL)
		return NULL;
	if((uint64_t) child == actual->child){
	//	out_printf("child: %d\n", child);
		unblockProcess(actual->father);
		WaitpidHistory ret = actual->next;
		//TODO: free
		return removeWaitpidHistoryRec(child, ret);
	}
	actual->next = removeWaitpidHistoryRec(child, actual->next);
	return actual;
}