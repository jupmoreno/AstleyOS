OUTPUT_FORMAT("binary")
ENTRY(init)
SECTIONS {
	.text 0x400000 : {
		shell_text = .;
		*(.text*)
		. = ALIGN(0x1000);
		shell_rodata = .;
		*(.rodata*)
	}
	.data ALIGN(0x1000) : {
		shell_data = .;
		*(.data*)
		shell_binary = .;
	}
	.bss ALIGN(0x1000) : {
		shell_bss = .;
		*(.bss*)
	}
	shell_end = .;
}
