#include <exceptions.h>
#include <kernel.h>
#include <paging.h>

#define _EXCEPTION_00_CODE "DE"
#define _EXCEPTION_00_DESC "Divide Error"
#define _EXCEPTION_00_SOURCE "DIV and IDIV instructions"
#define _EXCEPTION_00_HALT TRUE

#define _EXCEPTION_02_CODE "--"
#define _EXCEPTION_02_DESC "NMI Interrupt"
#define _EXCEPTION_02_SOURCE "Nonmaskable external interrupt"
#define _EXCEPTION_02_HALT TRUE

#define _EXCEPTION_03_CODE "BP"
#define _EXCEPTION_03_DESC "Breakpoint"
#define _EXCEPTION_03_SOURCE "INT 3 instruction"
#define _EXCEPTION_03_HALT TRUE

#define _EXCEPTION_04_CODE "OF"
#define _EXCEPTION_04_DESC "Overflow"
#define _EXCEPTION_04_SOURCE "INTO instruction"
#define _EXCEPTION_04_HALT TRUE

#define _EXCEPTION_05_CODE "DE"
#define _EXCEPTION_05_DESC "BOUND Range Exceeded"
#define _EXCEPTION_05_SOURCE "BOUND instruction"
#define _EXCEPTION_05_HALT TRUE

#define _EXCEPTION_06_CODE "UD"
#define _EXCEPTION_06_DESC "Invalid Opcode (Undefined Opcode)"
#define _EXCEPTION_06_SOURCE "UD2 instruction or reserved opcode"
#define _EXCEPTION_06_HALT TRUE

#define _EXCEPTION_07_CODE "NM"
#define _EXCEPTION_07_DESC "Device Not Available (No Math Coprocessor)"
#define _EXCEPTION_07_SOURCE "Floating-point or WAIT/FWAIT instruction"
#define _EXCEPTION_07_HALT TRUE

#define _EXCEPTION_08_CODE "DF"
#define _EXCEPTION_08_DESC "Double Fault"
#define _EXCEPTION_08_SOURCE "Any instruction that generates an exception, an NMI or an INTR"
#define _EXCEPTION_08_HALT TRUE

#define _EXCEPTION_09_CODE "--"
#define _EXCEPTION_09_DESC "Coprocessor Segment Overrun (reserved)"
#define _EXCEPTION_09_SOURCE "Floating-point instruction"
#define _EXCEPTION_09_HALT TRUE

#define _EXCEPTION_0A_CODE "TS"
#define _EXCEPTION_0A_DESC "Invalid TSS"
#define _EXCEPTION_0A_SOURCE "Task switch or TSS access"
#define _EXCEPTION_0A_HALT TRUE

#define _EXCEPTION_0B_CODE "NP"
#define _EXCEPTION_0B_DESC "Segment Not Present"
#define _EXCEPTION_0B_SOURCE "Loading segment registers or accessing system segments"
#define _EXCEPTION_0B_HALT TRUE

#define _EXCEPTION_0C_CODE "SS"
#define _EXCEPTION_0C_DESC "Stack-Segment Fault"
#define _EXCEPTION_0C_SOURCE "Stack operations and SS register loads"
#define _EXCEPTION_0C_HALT TRUE

#define _EXCEPTION_0D_CODE "GP"
#define _EXCEPTION_0D_DESC "General Protection"
#define _EXCEPTION_0D_SOURCE "Any memory reference and other protection checks"
#define _EXCEPTION_0D_HALT TRUE

#define _EXCEPTION_0E_CODE "PF"
#define _EXCEPTION_0E_DESC "Page Fault"
#define _EXCEPTION_0E_SOURCE "Any memory reference"
#define _EXCEPTION_0E_HALT TRUE

#define _EXCEPTION_10_CODE "MF"
#define _EXCEPTION_10_DESC "x87 FPU Floating-Point Error (Math Fault)"
#define _EXCEPTION_10_SOURCE "x87 FPU floating-point or WAIT/FWAIT instruction"
#define _EXCEPTION_10_HALT TRUE

#define _EXCEPTION_11_CODE "AC"
#define _EXCEPTION_11_DESC "Alignment Check"
#define _EXCEPTION_11_SOURCE "Any data reference in memory"
#define _EXCEPTION_11_HALT TRUE

#define _EXCEPTION_12_CODE "MC"
#define _EXCEPTION_12_DESC "Machine Check"
#define _EXCEPTION_12_SOURCE "Error codes (if any) and source are model dependent"
#define _EXCEPTION_12_HALT TRUE

#define _EXCEPTION_13_CODE "XM"
#define _EXCEPTION_13_DESC "SIMD Floating-Point Exception"
#define _EXCEPTION_13_SOURCE "SSE/SSE2/SSE3 floating-point instructions"
#define _EXCEPTION_13_HALT TRUE

#define _EXCEPTION_THROW(NUMBER) kernel_panic(_EXCEPTION_ ## NUMBER ## _CODE, \
											_EXCEPTION_ ## NUMBER ## _DESC, \
											_EXCEPTION_ ## NUMBER ## _SOURCE, \
											_EXCEPTION_ ## NUMBER ## _HALT)
#define _EXCEPTION_DEFINE(NUMBER) void exception_ ## NUMBER(void) { _EXCEPTION_THROW(NUMBER); } // TODO:

void exception_00(void) { // Divide Error: division by zero
	_EXCEPTION_THROW(00);
}

void exception_02(void) { // Non-maskable interrupt
	_EXCEPTION_THROW(02);
}

void exception_03(void) { // Breakpoint
	_EXCEPTION_THROW(03);
}

void exception_04(void) { // Overflow
	_EXCEPTION_THROW(04);
}

void exception_05(void) { // Bounds
	_EXCEPTION_THROW(05);
}

void exception_06(void) { // Invalid Opcode
	_EXCEPTION_THROW(06);
}

void exception_07(void) { // Coprocessor not available
	_EXCEPTION_THROW(07);
}

void exception_08(void) { // Double fault
	_EXCEPTION_THROW(08);
}

void exception_09(void) { // Coprocessor Segment Overrun (386 or earlier only)
	_EXCEPTION_THROW(09);
}

void exception_0A(void) { // Invalid Task State Segment
	_EXCEPTION_THROW(0A);
}

void exception_0B(void) { // Segment not present
	_EXCEPTION_THROW(0B);
}

void exception_0C(void) { // Stack Fault
	_EXCEPTION_THROW(0C);
}

void exception_0D(void) { // General protection fault
	_EXCEPTION_THROW(0D);
}

void exception_0E(uint64_t code, uint64_t fault_address) { // Page fault
	pferror_handler(code, fault_address);
	_EXCEPTION_THROW(0E);
}

void exception_10(void) { // Math Fault
	_EXCEPTION_THROW(10);
}

void exception_11(void) { // Alignment Check
	_EXCEPTION_THROW(11);
}

void exception_12(void) { // Machine Check
	_EXCEPTION_THROW(12);
}

void exception_13(void) { // SIMD Floating-Point Exception
	_EXCEPTION_THROW(13);
}
