/*
 * MicroCalculator - calc.c
 *
 * See calc.h for the grammar. Deliberately has no libc dependency
 * (no strtol/printf) so it links cleanly with -nostdlib on every
 * target.
 */

#include "calc.h"

struct calc_ctx {
    const char *p;
    calc_error_t err;
};

static void skip_ws(struct calc_ctx *c)
{
    while (*c->p == ' ' || *c->p == '\t') {
        c->p++;
    }
}

static long parse_expr(struct calc_ctx *c);

static long parse_factor(struct calc_ctx *c)
{
    int neg = 0;
    long v;

    skip_ws(c);
    if (*c->p == '+') {
        c->p++;
    } else if (*c->p == '-') {
        neg = 1;
        c->p++;
    }
    skip_ws(c);

    if (*c->p == '(') {
        c->p++;
        v = parse_expr(c);
        if (c->err != CALC_OK) {
            return 0;
        }
        skip_ws(c);
        if (*c->p != ')') {
            c->err = CALC_ERR_SYNTAX;
            return 0;
        }
        c->p++;
        return neg ? -v : v;
    }

    if (*c->p < '0' || *c->p > '9') {
        c->err = CALC_ERR_SYNTAX;
        return 0;
    }

    v = 0;
    while (*c->p >= '0' && *c->p <= '9') {
        v = v * 10 + (*c->p - '0');
        c->p++;
    }
    return neg ? -v : v;
}

static long parse_term(struct calc_ctx *c)
{
    long v = parse_factor(c);
    if (c->err != CALC_OK) {
        return 0;
    }

    for (;;) {
        skip_ws(c);
        if (*c->p == '*') {
            c->p++;
            long rhs = parse_factor(c);
            if (c->err != CALC_OK) {
                return 0;
            }
            v *= rhs;
        } else if (*c->p == '/') {
            c->p++;
            long rhs = parse_factor(c);
            if (c->err != CALC_OK) {
                return 0;
            }
            if (rhs == 0) {
                c->err = CALC_ERR_DIV_ZERO;
                return 0;
            }
            v /= rhs;
        } else {
            break;
        }
    }
    return v;
}

static long parse_expr(struct calc_ctx *c)
{
    long v = parse_term(c);
    if (c->err != CALC_OK) {
        return 0;
    }

    for (;;) {
        skip_ws(c);
        if (*c->p == '+') {
            c->p++;
            long rhs = parse_term(c);
            if (c->err != CALC_OK) {
                return 0;
            }
            v += rhs;
        } else if (*c->p == '-') {
            c->p++;
            long rhs = parse_term(c);
            if (c->err != CALC_OK) {
                return 0;
            }
            v -= rhs;
        } else {
            break;
        }
    }
    return v;
}

long calc_eval(const char *expr, calc_error_t *err)
{
    struct calc_ctx c;
    long v;

    c.p = expr;
    c.err = CALC_OK;

    skip_ws(&c);
    if (*c.p == '\0') {
        *err = CALC_ERR_EMPTY;
        return 0;
    }

    v = parse_expr(&c);

    if (c.err == CALC_OK) {
        skip_ws(&c);
        if (*c.p != '\0') {
            c.err = CALC_ERR_SYNTAX;
        }
    }

    *err = c.err;
    return v;
}

const char *calc_error_string(calc_error_t err)
{
    switch (err) {
    case CALC_OK:            return "ok";
    case CALC_ERR_SYNTAX:    return "syntax error";
    case CALC_ERR_DIV_ZERO:  return "division by zero";
    case CALC_ERR_EMPTY:     return "empty input";
    default:                 return "unknown error";
    }
}

void calc_ltoa(long val, char *buf)
{
    char tmp[24];
    int i = 0;
    int neg = 0;
    unsigned long uval;
    int j;

    if (val < 0) {
        neg = 1;
        uval = (unsigned long)(-(val + 1)) + 1UL; /* avoids overflow on LONG_MIN */
    } else {
        uval = (unsigned long)val;
    }

    if (uval == 0) {
        tmp[i++] = '0';
    }
    while (uval > 0) {
        tmp[i++] = (char)('0' + (uval % 10));
        uval /= 10;
    }

    j = 0;
    if (neg) {
        buf[j++] = '-';
    }
    while (i > 0) {
        buf[j++] = tmp[--i];
    }
    buf[j] = '\0';
}
