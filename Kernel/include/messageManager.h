#ifndef _messageManager_H_
#define _messageManager_H_

#include <define.h>

typedef struct blockedN {
	uint64_t id;
	struct blockedN * next;
}blockedN;

typedef blockedN * blockedNode;

 void block(uint64_t id);

 void unblock(uint64_t id);

 char isMQBlocked(uint64_t id);

#endif