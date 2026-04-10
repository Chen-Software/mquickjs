#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "mquickjs/mquickjs.h"

// Forward declaration of cabi_realloc (provided by wit-bindgen runtime)
void *cabi_realloc(void *ptr, size_t old_size, size_t old_align, size_t new_size);

// Minimal implementations of missing functions for WASI
JSValue js_date_now(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv) { return JS_UNDEFINED; }
JSValue js_print(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv) { return JS_UNDEFINED; }
JSValue js_performance_now(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv) { return JS_UNDEFINED; }
JSValue js_gc(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv) { return JS_UNDEFINED; }
JSValue js_load(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv) { return JS_UNDEFINED; }
JSValue js_setTimeout(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv) { return JS_UNDEFINED; }
JSValue js_clearTimeout(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv) { return JS_UNDEFINED; }

#include "mqjs_stdlib.h"
#include "generated/microquickjs.h"

/// Helper: Copy a string into WASI-managed memory for return to host.
static char *make_wasi_string(const char *src, size_t len) {
    if (!src) return NULL;
    char *out = (char *)cabi_realloc(NULL, 0, 1, len + 1);
    if (!out) return NULL;
    memcpy(out, src, len);
    out[len] = '\0';
    return out;
}

static uint8_t s_mem[4 * 1024 * 1024];
static JSContext *s_ctx = NULL;

static void ensure_context(void) {
    if (s_ctx) return;
    s_ctx = JS_NewContext(s_mem, sizeof(s_mem), &js_stdlib);
}

bool exports_microquickjs_eval(microquickjs_string_t *code, microquickjs_string_t *ret, microquickjs_string_t *err) {
    ensure_context();

    JSValue val = JS_Eval(s_ctx, (const char *)code->ptr, code->len, "<stdin>", JS_EVAL_RETVAL);

    const char *cstr;
    size_t clen;
    JSCStringBuf buf;

    if (JS_IsException(val)) {
        JSValue exc = JS_GetException(s_ctx);
        cstr = JS_ToCStringLen(s_ctx, &clen, exc, &buf);
        if (!cstr) {
            static const char fallback[] = "Error: unknown exception";
            err->ptr = (uint8_t *)make_wasi_string(fallback, sizeof(fallback) - 1);
            err->len = sizeof(fallback) - 1;
            return false;
        }
        err->ptr = (uint8_t *)make_wasi_string(cstr, clen);
        err->len = clen;
        return false;
    }

    cstr = JS_ToCStringLen(s_ctx, &clen, val, &buf);
    if (!cstr) {
        static const char fallback[] = "undefined";
        ret->ptr = (uint8_t *)make_wasi_string(fallback, sizeof(fallback) - 1);
        ret->len = sizeof(fallback) - 1;
        return true;
    }
    ret->ptr = (uint8_t *)make_wasi_string(cstr, clen);
    ret->len = clen;
    return true;
}
