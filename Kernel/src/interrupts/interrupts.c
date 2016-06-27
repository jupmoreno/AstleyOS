#include <interrupts.h>
#include <pic.h>
#include <ksyscalls.h>
#include <output.h>

extern void _interrupt_set(void);
extern void _interrupt_clear(void);

extern uint64_t pit_trigger(uint64_t stackF);
extern void keyboard_trigger(unsigned char scancode);

uint64_t interrupt_20(uint64_t stackF);
void interrupt_21(unsigned char scancode);
syscall_st * interrupt_80(int syscall);

void interrupt_set(void) {
	_interrupt_set();

	log("<STI>\n");
}

void interrupt_clear(void) {
	_interrupt_clear();

	log("<CLI>\n");
}

uint64_t interrupt_20(uint64_t stackF) {
	uint64_t stackFaux;
	stackFaux = pit_trigger(stackF);
	pic_irq_eoi(0);
	return stackFaux;
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