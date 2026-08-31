/*
 * PMicroCalculator - main.c
 *
 * Same REPL as the MicroCalculator projects,
 * this time on a Raspberry Pi Pico (RP2040, dual-core Cortex-M0+).
 *
 * Uses the Pico SDK's stdio layer (pico/stdlib.h) rather than poking
 * UART or USB registers directly -- CMakeLists.txt enables BOTH the
 * USB-CDC serial port and the UART0 pins (GPIO0 TX / GPIO1 RX) as
 * stdio backends, so this REPL is reachable either way without any
 * source change.
 */

#include <stddef.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "calc.h"

#define LINE_MAX 96

static void read_line(char *buf, size_t max_len)
{
    size_t idx = 0;

    for (;;) {
        char c = (char)getchar();

        if (c == '\r' || c == '\n') {
            printf("\r\n");
            buf[idx] = '\0';
            return;
        }

        if (c == 0x7f || c == 0x08) { /* DEL or backspace */
            if (idx > 0) {
                idx--;
                printf("\b \b");
            }
            continue;
        }

        if (idx < max_len - 1) {
            buf[idx++] = c;
            putchar(c); /* local echo */
        }
    }
}

int main(void)
{
    char line[LINE_MAX];

    stdio_init_all();

    printf("\r\n");
    printf("========================================\r\n");
    printf(" MicroCalculator -- Raspberry Pi Pico (RP2040)\r\n");
    printf("========================================\r\n");
    printf("Type an expression with + - * / and ( ), e.g. (3 + 4) * 2\r\n\r\n");
    fflush(stdout);

    for (;;) {
        calc_error_t err;
        long result;
        char num_buf[24];

        printf("calc> ");
        fflush(stdout);
        read_line(line, LINE_MAX);

        result = calc_eval(line, &err);

        if (err == CALC_ERR_EMPTY) {
            continue;
        }

        if (err == CALC_OK) {
            calc_ltoa(result, num_buf);
            printf("= %s\r\n\r\n", num_buf);
        } else {
            printf("error: %s\r\n\r\n", calc_error_string(err));
        }
        fflush(stdout);
    }

    return 0; /* never reached */
}
