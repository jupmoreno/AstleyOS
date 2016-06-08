#include <liba.h>

extern uint32_t _ram_amount();

unsigned int ram_amount() {
	return _ram_amount();
}
