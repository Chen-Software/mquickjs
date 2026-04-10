#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "mquickjs/mquickjs.h"

// Minimal implementations of missing functions for WASI
JSValue js_date_now(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv) { return JS_UNDEFINED; }
JSValue js_print(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv) { return JS_UNDEFINED; }
JSValue js_performance_now(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv) { return JS_UNDEFINED; }
JSValue js_gc(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv) { return JS_UNDEFINED; }
JSValue js_load(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv) { return JS_UNDEFINED; }
JSValue js_setTimeout(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv) { return JS_UNDEFINED; }
JSValue js_clearTimeout(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv) { return JS_UNDEFINED; }

#include "generated/microquickjs.h"
#include "mqjs_stdlib.h"

// cabi_realloc is provided by wit-bindgen's microquickjs.c
void *cabi_realloc(void *ptr, size_t old_size, size_t align, size_t new_size);

static uint8_t s_mem[4 * 1024 * 1024];
static JSContext *s_ctx = NULL;

static void ensure_context(void) {
    if (s_ctx) return;
    s_ctx = JS_NewContext(s_mem, sizeof(s_mem), &js_stdlib);
}

static char *wasi_strndup(const char *s, size_t n) {
    char *p = cabi_realloc(NULL, 0, 1, n + 1);
    memcpy(p, s, n);
    p[n] = '\0';
    return p;
}

void exports_microquickjs_eval(
    microquickjs_string_t *code,
    microquickjs_string_t *ret)
{
    ensure_context();

    JSValue val = JS_Eval(s_ctx,
                          (const char *)code->ptr,
                          code->len,
                          "<eval>",
                          JS_EVAL_RETVAL);

    const char *cstr;
    size_t clen;
    JSCStringBuf buf;

    if (JS_IsException(val)) {
        JSValue exc = JS_GetException(s_ctx);
        cstr = JS_ToCStringLen(s_ctx, &clen, exc, &buf);
        if (!cstr) {
            static const char fallback[] = "Error: unknown exception";
            ret->ptr = (uint8_t *)wasi_strndup(fallback, sizeof(fallback) - 1);
            ret->len = sizeof(fallback) - 1;
            return;
        }
        if (clen < 5 || memcmp(cstr, "Error", 5) != 0) {
            static const char prefix[] = "Error: ";
            size_t total = sizeof(prefix) - 1 + clen;
            char *out = cabi_realloc(NULL, 0, 1, total + 1);
            memcpy(out, prefix, sizeof(prefix) - 1);
            memcpy(out + sizeof(prefix) - 1, cstr, clen);
            out[total] = '\0';
            ret->ptr = (uint8_t *)out;
            ret->len = total;
        } else {
            ret->ptr = (uint8_t *)wasi_strndup(cstr, clen);
            ret->len = clen;
        }
        return;
    }

    cstr = JS_ToCStringLen(s_ctx, &clen, val, &buf);
    if (!cstr) {
        static const char fallback[] = "undefined";
        ret->ptr = (uint8_t *)wasi_strndup(fallback, sizeof(fallback) - 1);
        ret->len = sizeof(fallback) - 1;
        return;
    }
    ret->ptr = (uint8_t *)wasi_strndup(cstr, clen);
    ret->len = clen;
}
