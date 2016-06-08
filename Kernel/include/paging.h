#ifndef _PAGING_H_
#define _PAGING_H_

#include <kdefine.h>

int paging_init(void);

void pferror_handler(uint64_t code, uint64_t fault_address);

#endif
