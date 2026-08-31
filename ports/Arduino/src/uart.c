/*
 * ArduinoMicroCalculator - uart.c
 *
 * Straight register-level USART0 driver, 8N1, no interrupts (polled).
 * Baud rate defaults to 115200 using double-speed mode (U2X0=1), which
 * gives ~2.1% clock error at F_CPU=16MHz -- the same trick the Arduino
 * core itself uses, and well within what a UART can tolerate. All four
 * target boards (Uno, Nano, Mega 2560, ATmega1280 Mega) run their
 * on-board crystal at 16MHz, so F_CPU=16000000UL (set by the Makefile)
 * is correct for all of them.
 */

#include "uart.h"
#include <avr/io.h>

#ifndef F_CPU
#error "F_CPU must be defined on the command line (see Makefile)"
#endif

#ifndef UART_BAUD
#define UART_BAUD 115200UL
#endif

/* Double-speed (U2X0=1) UBRR formula. */
#define UART_UBRR ((F_CPU) / (8UL * (UART_BAUD)) - 1)

void uart_init(void)
{
    UBRR0H = (uint8_t)(UART_UBRR >> 8);
    UBRR0L = (uint8_t)(UART_UBRR & 0xFF);

    UCSR0A = (1 << U2X0);                    /* double-speed mode */
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);    /* enable TX and RX  */
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);  /* 8 data bits, 1 stop, no parity */
}

void uart_putc(char c)
{
    while (!(UCSR0A & (1 << UDRE0))) {
        /* wait for the transmit data register to be empty */
    }
    UDR0 = (uint8_t)c;
}

void uart_puts(const char *s)
{
    while (*s) {
        if (*s == '\n') {
            uart_putc('\r');
        }
        uart_putc(*s++);
    }
}

int uart_rx_ready(void)
{
    return (UCSR0A & (1 << RXC0)) != 0;
}

char uart_getc(void)
{
    while (!uart_rx_ready()) {
        /* wait for a received byte */
    }
    return (char)UDR0;
}
