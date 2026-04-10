#ifndef MQUICKJS_WASM_SETJMP_H
#define MQUICKJS_WASM_SETJMP_H

#include <stdint.h>
#include <stdlib.h>

// A minimal setjmp/longjmp stub for WASM when the proposal is not used.
// MicroQuickJS uses this for syntax error reporting.
// Without the proposal, we can't truly jump, so we'll have to abort if longjmp is hit.

typedef struct {
    uint32_t dummy;
} jmp_buf[1];

static inline int setjmp(jmp_buf env) {
    (void)env;
    return 0; // Always returns 0 for the first call
}

static inline void __attribute__((noreturn)) longjmp(jmp_buf env, int val) {
    (void)env;
    (void)val;
    // Since we can't actually jump, we abort.
    // In a real implementation we'd need the Wasm Exception Handling proposal.
    abort();
}

#endif
