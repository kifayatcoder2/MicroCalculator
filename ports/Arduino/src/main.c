/*
 * ArduinoMicroCalculator - main.c
 *
 * Same REPL as the MicroCalculator project, retargeted to real
 * AVR hardware: reads a line from USART0 (the board's USB-serial
 * port), evaluates it as an arithmetic expression, prints the result.
 *
 * Open the Arduino Serial Monitor (or `screen`/PuTTY/minicom) at
 * 115200 baud, line ending "Newline" or "Both NL & CR".
 */

#include "uart.h"
#include "calc.h"
#include <stddef.h>

#ifndef BOARD_NAME
#define BOARD_NAME "unknown board"
#endif

#define LINE_MAX 64

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

int main(void)
{
    uart_init();

    uart_puts("\r\n");
    uart_puts("========================================\r\n");
    uart_puts(" MicroCalculator -- " BOARD_NAME "\r\n");
    uart_puts("========================================\r\n");
    uart_puts("Type an expression with + - * / and ( ), e.g. (3 + 4) * 2\r\n\r\n");

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

    return 0; /* never reached */
}
