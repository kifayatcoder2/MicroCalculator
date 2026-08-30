/*
 * MicroCalculator - uart.c
 *
 * CMSDK APB UART register layout (confirmed against QEMU's own model,
 * hw/char/cmsdk-apb-uart.c):
 *
 *   0x00  DATA       (r/w) byte to transmit / received byte
 *   0x04  STATE      (r/w) bit0 TXFULL, bit1 RXFULL, bit2 TXOVERRUN,
 *                            bit3 RXOVERRUN
 *   0x08  CTRL       (r/w) bit0 TX_EN, bit1 RX_EN, bit2 TX_INTEN,
 *                            bit3 RX_INTEN, bit4 TXO_INTEN, bit5 RXO_INTEN
 *   0x0C  INTSTATUS / INTCLEAR
 *   0x10  BAUDDIV    (r/w) clock divider, minimum useful value is 16
 */

#include "uart.h"
#include <stdint.h>

#define UART_DATA_OFF     0x00u
#define UART_STATE_OFF    0x04u
#define UART_CTRL_OFF     0x08u
#define UART_BAUDDIV_OFF  0x10u

#define UART_STATE_TXFULL (1u << 0)
#define UART_STATE_RXFULL (1u << 1)

#define UART_CTRL_TXEN    (1u << 0)
#define UART_CTRL_RXEN    (1u << 1)

static inline volatile uint32_t *uart_reg(uint32_t off)
{
    return (volatile uint32_t *)(uintptr_t)(UART0_BASE + off);
}

void uart_init(void)
{
    *uart_reg(UART_BAUDDIV_OFF) = (uint32_t)UART_BAUDDIV_VALUE;
    *uart_reg(UART_CTRL_OFF) = UART_CTRL_TXEN | UART_CTRL_RXEN;
}

void uart_putc(char c)
{
    while (*uart_reg(UART_STATE_OFF) & UART_STATE_TXFULL) {
        /* wait for space in the transmit buffer */
    }
    *uart_reg(UART_DATA_OFF) = (uint32_t)(uint8_t)c;
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
    return (*uart_reg(UART_STATE_OFF) & UART_STATE_RXFULL) != 0;
}

char uart_getc(void)
{
    while (!uart_rx_ready()) {
        /* wait for a received byte */
    }
    return (char)(*uart_reg(UART_DATA_OFF) & 0xFFu);
}
