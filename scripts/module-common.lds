/*
 * Common module linker script, always used when linking a module.
 * Archs are free to supply their own linker scripts.  ld will
 * combine them automatically.
 */
SECTIONS {
	/DISCARD/ : { *(.discard) }
	.text : { *(.text*) }
	.data : { *(.data*) }
	.rodata : { *(.rodata*) }
	.bss  : { *(.bss*) }
}
