/*
 * MicroCalculator - uart.h
 *
 * Driver for the ARM CMSDK APB UART, as modelled by QEMU for every
 * MPS2 board in this project (mps2-an385/an386/an500/an505). The
 * register layout is identical on all four; only the base address
 * differs, and that is supplied at build time via -DUART0_BASE=...
 * (see the top-level Makefile).
 */

#ifndef MICROCALC_UART_H
#define MICROCALC_UART_H

#include <stddef.h>

#ifndef UART0_BASE
#error "UART0_BASE must be defined on the command line (see Makefile)"
#endif

#ifndef UART_BAUDDIV_VALUE
#error "UART_BAUDDIV_VALUE must be defined on the command line (see Makefile)"
#endif

void uart_init(void);
void uart_putc(char c);
void uart_puts(const char *s);
int  uart_rx_ready(void);
char uart_getc(void);

#endif /* MICROCALC_UART_H */
