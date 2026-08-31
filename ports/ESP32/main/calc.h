/*
 * MicroCalculator - calc.h
 *
 * Tiny recursive-descent integer expression evaluator.
 * Grammar:
 *   expr   := term (('+' | '-') term)*
 *   term   := factor (('*' | '/') factor)*
 *   factor := ['+'|'-'] (NUMBER | '(' expr ')')
 */

#ifndef MICROCALC_CALC_H
#define MICROCALC_CALC_H

typedef enum {
    CALC_OK = 0,
    CALC_ERR_SYNTAX,
    CALC_ERR_DIV_ZERO,
    CALC_ERR_EMPTY,
} calc_error_t;

/* Evaluate a null-terminated expression string. On success *err is set
 * to CALC_OK and the result is returned; on failure *err describes the
 * problem and the return value is undefined. */
long calc_eval(const char *expr, calc_error_t *err);

const char *calc_error_string(calc_error_t err);

/* Minimal signed long -> decimal string, no libc dependency. buf must
 * be at least 24 bytes. */
void calc_ltoa(long val, char *buf);

#endif /* MICROCALC_CALC_H */
