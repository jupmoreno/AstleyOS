#include <system.h>
#include <define.h>

extern void _halt();

void system_halt() {
	_halt();
}
