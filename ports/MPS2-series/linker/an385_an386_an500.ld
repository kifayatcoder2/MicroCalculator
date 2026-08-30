/*
 * Linker script for mps2-an385 / mps2-an386 / mps2-an500.
 *
 * Per QEMU's hw/arm/mps2.c, all three boards map "ZBT SSRAM1" as a
 * 4 MB region at 0x00000000, and this is where the CPU's reset
 * vector fetch (SP at offset 0, PC at offset 4) always looks.
 */

MEMORY
{
    RAM (rwx) : ORIGIN = 0x00000000, LENGTH = 4M
}

ENTRY(Reset_Handler)

SECTIONS
{
    .isr_vector : {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
    } > RAM

    .text : {
        . = ALIGN(4);
        *(.text*)
        *(.rodata*)
        . = ALIGN(4);
    } > RAM

    .data : {
        . = ALIGN(4);
        _sdata = .;
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } > RAM

    _sidata = _sdata; /* LMA == VMA: nothing to relocate from flash */

    .bss (NOLOAD) : {
        . = ALIGN(4);
        _sbss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    } > RAM

    /DISCARD/ : { *(.comment) *(.ARM.attributes) }
}

/* Stack starts at the top of RAM and grows down; standard for a
 * single-region bare-metal image with no heap. */
_estack = ORIGIN(RAM) + LENGTH(RAM);
