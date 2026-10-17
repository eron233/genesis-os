ENTRY(_start)
SECTIONS {
    . = 1M;
    .multiboot : { KEEP(*(.multiboot)) }
    . = ALIGN(4096);
    .text : { *(.text .text.*) }
    . = ALIGN(4096);
    .rodata : { *(.rodata .rodata.*) }
    . = ALIGN(4096);
    .data : { *(.data .data.*) }
    . = ALIGN(4096);
    .bss : { *(COMMON) *(.bss .bss.*) }
    /DISCARD/ : { *(.comment) *(.note*) *(.eh_frame*) }
}
