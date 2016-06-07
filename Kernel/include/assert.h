#ifndef _ASSERT_H_
#define _ASSERT_H_

#include <kdefine.h>
#include <kernel.h>

#define assert(exp) if(!(exp)) { \
	kernel_panic("AS", \
				"Assert Error", \
				__FILE__":"STRINGNIFY_VALUE(__LINE__), \
				TRUE); \
}

#endif
