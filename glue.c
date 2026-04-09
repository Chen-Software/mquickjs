#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "microquickjs.h"
#include "mquickjs.h"

// cabi_realloc is provided by wit-bindgen runtime
void *cabi_realloc(void *ptr, size_t old_size, size_t align, size_t new_size);

// Stubs for functions used in mqjs_stdlib.h
JSValue js_print(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv) { return JS_UNDEFINED; }
JSValue js_date_now(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv) { return JS_UNDEFINED; }
JSValue js_performance_now(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv) { return JS_UNDEFINED; }
JSValue js_gc(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv) { return JS_UNDEFINED; }
JSValue js_load(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv) { return JS_UNDEFINED; }
JSValue js_setTimeout(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv) { return JS_UNDEFINED; }
JSValue js_clearTimeout(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv) { return JS_UNDEFINED; }

#include "mqjs_stdlib.h"

static JSContext *ctx = NULL;

bool exports_microquickjs_eval(microquickjs_string_t *code, microquickjs_string_t *ok, microquickjs_string_t *err) {
    if (ctx == NULL) {
        size_t mem_size = 16 << 20;
        uint8_t *mem_buf = malloc(mem_size);
        ctx = JS_NewContext(mem_buf, mem_size, &js_stdlib);
    }

    // Convert WIT string to null-terminated C string
    char *src = malloc(code->len + 1);
    memcpy(src, code->ptr, code->len);
    src[code->len] = '\0';

    JSValue val = JS_Eval(ctx, src, code->len, "<eval>", 0);
    free(src);

    const char *result_str = NULL;

    if (JS_IsException(val)) {
        JSValue exc = JS_GetException(ctx);
        JSCStringBuf buf;
        result_str = JS_ToCString(ctx, exc, &buf);

        size_t elen = strlen(result_str) + 8;
        char *ebuf = cabi_realloc(NULL, 0, 1, elen);
        snprintf(ebuf, elen, "Error: %s", result_str);

        err->ptr = (uint8_t *)ebuf;
        err->len = strlen(ebuf);
        return false; // Result is false for error
    } else {
        JSCStringBuf buf;
        result_str = JS_ToCString(ctx, val, &buf);
        if (!result_str) result_str = "undefined";

        size_t len = strlen(result_str);
        char *out = cabi_realloc(NULL, 0, 1, len + 1);
        memcpy(out, result_str, len + 1);

        ok->ptr = (uint8_t *)out;
        ok->len = len;
        return true; // Result is true for success
    }
}
