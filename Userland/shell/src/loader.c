#include <define.h>
#include <memory.h>

// Module setup
extern uint8_t shell_text;
extern uint8_t shell_rodata;
extern uint8_t shell_data;
extern uint8_t shell_bss;
extern uint8_t shell_binary;
extern uint8_t shell_end;
// ^^ Module setup ^^

extern int main();

void init() {
	// Clear BSS
	memset(&shell_bss, 0, &shell_end - &shell_bss);

	// Call shell's main
	main();
}
