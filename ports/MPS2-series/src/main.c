/*
 * MicroCalculator - main.c
 *
 * Bare-metal REPL: reads a line of text from UART0, evaluates it as
 * an arithmetic expression, and prints the result back over UART0.
 *
 * Runs unmodified on:
 *   qemu-system-arm -M mps2-an385 -cpu cortex-m3  -kernel build/an385/microcalc.elf -nographic
 *   qemu-system-arm -M mps2-an386 -cpu cortex-m4  -kernel build/an386/microcalc.elf -nographic
 *   qemu-system-arm -M mps2-an500 -cpu cortex-m7  -kernel build/an500/microcalc.elf -nographic
 *   qemu-system-arm -M mps2-an505 -cpu cortex-m33 -kernel build/an505/microcalc.elf -nographic
 */

#include "uart.h"
#include "calc.h"
#include <stddef.h>

#ifndef BOARD_NAME
#define BOARD_NAME "unknown board"
#endif

#define LINE_MAX 96

static char line_buf[LINE_MAX];

static void read_line(char *buf, size_t max_len)
{
    size_t idx = 0;

    for (;;) {
        char c = uart_getc();

        if (c == '\r' || c == '\n') {
            uart_puts("\r\n");
            buf[idx] = '\0';
            return;
        }

        if (c == 0x7f || c == 0x08) { /* DEL or backspace */
            if (idx > 0) {
                idx--;
                uart_puts("\b \b");
            }
            continue;
        }

        if (idx < max_len - 1) {
            buf[idx++] = c;
            uart_putc(c); /* local echo */
        }
    }
}

void main(void)
{
    uart_init();

    uart_puts("\r\n");
    uart_puts("========================================\r\n");
    uart_puts(" MicroCalculator -- " BOARD_NAME "\r\n");
    uart_puts(" bare-metal, running under QEMU\r\n");
    uart_puts("========================================\r\n");
    uart_puts("Type an expression with + - * / and ( ), e.g. (3 + 4) * 2\r\n");
    uart_puts("Press Ctrl-A then X to leave QEMU.\r\n\r\n");

    for (;;) {
        calc_error_t err;
        long result;
        char num_buf[24];

        uart_puts("calc> ");
        read_line(line_buf, LINE_MAX);

        result = calc_eval(line_buf, &err);

        if (err == CALC_ERR_EMPTY) {
            continue;
        }

        if (err == CALC_OK) {
            calc_ltoa(result, num_buf);
            uart_puts("= ");
            uart_puts(num_buf);
            uart_puts("\r\n\r\n");
        } else {
            uart_puts("error: ");
            uart_puts(calc_error_string(err));
            uart_puts("\r\n\r\n");
        }
    }
}
