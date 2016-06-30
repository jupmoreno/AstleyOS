#ifndef _WAITPIDHISTORY_H_
#define _WAITPIDHISTORY_H_
#include <stdint.h>

typedef struct waitpidHistory{
	uint64_t father;
	uint64_t child;
	struct waitpidHistory* next;
}waitpidHistory;

typedef struct waitpidHistory* WaitpidHistory;




#endif