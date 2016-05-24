#include <interrupts.h>
#include <define.h>
#include <pic.h>
#include <ksyscalls.h>
#include <log.h>
#include <output.h>

extern void _interrupt_set();
extern void _interrupt_clear();

extern void pit_trigger();
extern void keyboard_trigger(unsigned char scancode);

void interrupt_20();
void interrupt_21(unsigned char scancode);
syscall_st * interrupt_80(int syscall);

void interrupt_set() {
	_interrupt_set();

	#ifdef _DEGUB_ENABLED
	log("<STI>\n");
	#endif
}

void interrupt_clear() {
	_interrupt_clear();
	
	#ifdef _DEGUB_ENABLED
	log("<CLI>\n");
	#endif
}

void interrupt_20() {
	pit_trigger();
	pic_irq_eoi(0);
}

void interrupt_21(unsigned char scancode) {
	keyboard_trigger(scancode);
	pic_irq_eoi(1);
}

syscall_st * interrupt_80(int syscall) {
	if(syscall < 0 || syscall >= _SYSCALLS_SIZE) {
		return NULL;
	}

	return syscalls_table[syscall];
}
