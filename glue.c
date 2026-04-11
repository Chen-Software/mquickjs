#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "generated/microquickjs.h"
#include "mquickjs.h"

// External declarations for symbols used in mqjs_stdlib.h
JSValue js_print(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv);
JSValue js_gc(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv);
JSValue js_date_now(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv);
JSValue js_performance_now(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv);
JSValue js_load(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv);
JSValue js_setTimeout(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv);
JSValue js_clearTimeout(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv);

#include "mqjs_stdlib.h"

// Forward declaration of cabi_realloc (provided by wit-bindgen runtime)
void *cabi_realloc(void *ptr, size_t old_size, size_t old_align, size_t new_size);

static uint8_t s_mem[4 * 1024 * 1024];
static JSContext *s_ctx = NULL;

static void ensure_context(void) {
    if (s_ctx) return;
    s_ctx = JS_NewContext(s_mem, sizeof(s_mem), &js_stdlib);
}

static char *make_wasi_string(const char *src, size_t len) {
    if (!src) return NULL;
    char *out = (char *)cabi_realloc(NULL, 0, 1, len + 1);
    if (!out) return NULL;
    memcpy(out, src, len);
    out[len] = '\0';
    return out;
}

bool exports_microquickjs_eval(microquickjs_string_t *code, microquickjs_string_t *ok, microquickjs_string_t *err) {
    ensure_context();
    JSValue result = JS_Eval(s_ctx, (const char *)code->ptr, code->len, "<eval>", JS_EVAL_RETVAL);
    JSCStringBuf buf;
    if (JS_IsException(result)) {
        JSValue exception = JS_GetException(s_ctx);
        const char *exc_str = JS_ToCString(s_ctx, exception, &buf);
        if (!exc_str) exc_str = "Unknown error";
        err->ptr = (uint8_t *)make_wasi_string(exc_str, strlen(exc_str));
        err->len = strlen(exc_str);
        return false;
    }
    const char *result_str = JS_ToCString(s_ctx, result, &buf);
    if (!result_str) result_str = "undefined";
    ok->ptr = (uint8_t *)make_wasi_string(result_str, strlen(result_str));
    ok->len = strlen(result_str);
    return true;
}
