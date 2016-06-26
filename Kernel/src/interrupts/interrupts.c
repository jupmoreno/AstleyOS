#include <interrupts.h>
#include <pic.h>
#include <ksyscalls.h>
#include <output.h>

extern void _interrupt_set(void);
extern void _interrupt_clear(void);

extern void pit_trigger(uint64_t stackF);
extern void keyboard_trigger(unsigned char scancode);

void interrupt_20(uint64_t stackF);
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

void interrupt_20(uint64_t stackF) {
	pit_trigger(stackF);
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
