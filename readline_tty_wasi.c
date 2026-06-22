#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include "readline_tty.h"

int readline_tty_init(void) { return 80; }
void term_printf(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
}
void term_flush(void) { fflush(stdout); }
const char *readline_tty(ReadlineState *s, const char *prompt, BOOL multi_line) { return NULL; }
BOOL readline_is_interrupted(void) { return 0; }
