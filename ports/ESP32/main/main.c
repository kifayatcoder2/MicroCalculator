/*
 * ESP32MicroCalculator - main.c
 *
 * Same REPL as the MicroCalculator projects, this
 * time built with ESP-IDF and targeting the ESP32 (original), ESP32-S3,
 * and ESP32-P4.
 *
 * Deliberately does NOT touch UART or USB-Serial/JTAG registers
 * directly. Those three chips don't share a console peripheral the
 * way the AVR boards shared USART0: the original ESP32 has no native
 * USB and talks to a host through UART0 via an external USB-UART
 * bridge chip, while the S3 and P4 default to their built-in native
 * USB Serial/JTAG controller instead. ESP-IDF's console/VFS layer is
 * exactly the abstraction that hides this difference -- `idf.py
 * set-target <chip>` picks the right console peripheral and wires it
 * up to stdin/stdout automatically, so this file is identical for all
 * three targets.
 */

#include <stdio.h>
#include <string.h>
#include "sdkconfig.h"
#include "calc.h"

#define LINE_MAX 96

static void strip_eol(char *s)
{
    size_t len = strlen(s);
    while (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r')) {
        s[--len] = '\0';
    }
}

void app_main(void)
{
    char line[LINE_MAX];

    printf("\r\n");
    printf("========================================\r\n");
    printf(" MicroCalculator -- %s\r\n", CONFIG_IDF_TARGET);
    printf("========================================\r\n");
    printf("Type an expression with + - * / and ( ), e.g. (3 + 4) * 2\r\n\r\n");
    fflush(stdout);

    for (;;) {
        printf("calc> ");
        fflush(stdout);

        if (fgets(line, sizeof(line), stdin) == NULL) {
            continue;
        }
        strip_eol(line);

        calc_error_t err;
        long result = calc_eval(line, &err);

        if (err == CALC_ERR_EMPTY) {
            continue;
        }

        if (err == CALC_OK) {
            char num_buf[24];
            calc_ltoa(result, num_buf);
            printf("= %s\r\n\r\n", num_buf);
        } else {
            printf("error: %s\r\n\r\n", calc_error_string(err));
        }
        fflush(stdout);
    }
}
