OUTPUT_FORMAT("binary")
ENTRY(loader)
SECTIONS {
	.text 0x0000000020141000 : {
		kernel_text = .;
		*(.text*)
		. = ALIGN(0x1000);
		kernel_rodata = .;
		*(.rodata*)
	}
	.data ALIGN(0x1000) : AT(ADDR(.data)) {
		kernel_data = .;
		*(.data*)
		kernel_binary = .;
	}
	.bss ALIGN(0x1000) : AT(ADDR(.bss)) {
		kernel_bss = .;
		*(.bss*)
		*(EXCLUDE_FILE (*.o) COMMON)
	}
	. = ALIGN(0x1000);
	kernel_end = .;
}
