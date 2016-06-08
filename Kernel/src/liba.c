#include <liba.h>

extern uint32_t _ram_amount(void);

unsigned int ram_amount(void) {
	return _ram_amount();
}
