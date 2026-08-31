/*
 * ArduinoMicroCalculator - uart.h
 *
 * Driver for USART0 -- the hardware serial port wired to the USB-serial
 * chip (and to the RX0/TX0 pins) on every board this project targets:
 * Uno, Nano, Mega 2560, and the ATmega1280 Mega. ATmega328P has one
 * USART; ATmega1280/2560 have four (USART0-3), but USART0's registers
 * have identical names and bit layout on all three parts, so this file
 * is compiled unmodified for every target -- only F_CPU and the chip
 * selected by -mmcu differ, both supplied by the Makefile.
 */

#ifndef MICROCALC_UART_H
#define MICROCALC_UART_H

void uart_init(void);
void uart_putc(char c);
void uart_puts(const char *s);
int  uart_rx_ready(void);
char uart_getc(void);

#endif /* MICROCALC_UART_H */
