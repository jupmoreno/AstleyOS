#include <system.h>
#include <define.h>

extern void _halt(void);

void system_halt(void) {
	_halt();
}
