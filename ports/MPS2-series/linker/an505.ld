/*
 * Linker script for mps2-an505 (Cortex-M33 / ARMv8-M with TrustZone).
 *
 * QEMU's hw/arm/mps2-tz.c sets init-svtor = 0x10000000 for this board,
 * i.e. the CPU's Secure reset vector fetch (SP at offset 0, PC at
 * offset 4) reads from 0x10000000 -- the Secure alias of "ssram-0"
 * (whose Non-secure alias sits at 0x00000000). This firmware never
 * touches the SAU or drops to Non-secure state, so it stays entirely
 * in this 4 MB Secure RAM region and uses Secure peripheral aliases
 * (see UART0_BASE = 0x50200000 in the Makefile).
 */

MEMORY
{
    RAM (rwx) : ORIGIN = 0x10000000, LENGTH = 4M
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

/* Stack starts at the top of RAM and grows down. */
_estack = ORIGIN(RAM) + LENGTH(RAM);
