/*
 * MicroCalculator - startup.c
 *
 * Minimal Cortex-M startup code: exception vector table + Reset_Handler.
 *
 * Only the 16 core-architecture exception vectors are defined. No
 * peripheral (external) interrupts are ever enabled by this firmware
 * (we poll the UART), so external IRQ vector slots are never fetched
 * and do not need to be populated. This keeps startup.c identical for
 * every target (Cortex-M3 / M4 / M7 / M33) even though each board
 * exposes a different number of external interrupt lines.
 */

#include <stdint.h>

/* Symbols provided by the linker script (see linker/*.ld) */
extern uint32_t _estack;
extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;

extern void main(void);

void Reset_Handler(void);
void Default_Handler(void);

void NMI_Handler(void)         __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void)   __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void)   __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void)    __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void)  __attribute__((weak, alias("Default_Handler")));
/* SecureFault only exists on ARMv8-M (Cortex-M33 / AN505); harmless
 * as an unused weak symbol on the other (ARMv7-M) targets. */
void SecureFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void)         __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void)    __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void)      __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void)     __attribute__((weak, alias("Default_Handler")));

typedef void (*isr_t)(void);

__attribute__((section(".isr_vector"), used))
const isr_t vector_table[16] = {
    (isr_t)&_estack,       /* 0:  Initial stack pointer            */
    Reset_Handler,          /* 1:  Reset                            */
    NMI_Handler,             /* 2:  NMI                              */
    HardFault_Handler,       /* 3:  Hard fault                       */
    MemManage_Handler,       /* 4:  MPU fault (v7M) / MemManage      */
    BusFault_Handler,        /* 5:  Bus fault                        */
    UsageFault_Handler,      /* 6:  Usage fault                      */
    SecureFault_Handler,     /* 7:  SecureFault (v8-M only)          */
    0,                       /* 8:  Reserved                         */
    0,                       /* 9:  Reserved                         */
    0,                       /* 10: Reserved                         */
    SVC_Handler,             /* 11: SVCall                           */
    DebugMon_Handler,        /* 12: Debug monitor                    */
    0,                       /* 13: Reserved                         */
    PendSV_Handler,          /* 14: PendSV                           */
    SysTick_Handler,         /* 15: SysTick                          */
};

void Reset_Handler(void)
{
    uint32_t *src = &_sidata;
    uint32_t *dst = &_sdata;

    /* Copy initialised data (on these boards LMA == VMA, so this is a
     * harmless no-op copy; kept for portability). */
    while (dst < &_edata) {
        *dst++ = *src++;
    }

    /* Zero-initialise .bss */
    dst = &_sbss;
    while (dst < &_ebss) {
        *dst++ = 0;
    }

    main();

    /* main() never returns, but just in case: */
    for (;;) {
        __asm__ volatile ("wfi");
    }
}

void Default_Handler(void)
{
    for (;;) {
        __asm__ volatile ("nop");
    }
}
