#ifndef _KERNEL_H_
#define _KERNEL_H_

#include <define.h>

void kernel_panic(const char * code, const char * desc, const char * source, const int halt);

#endif