#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "microquickjs.h"
#include "mquickjs.h"

// cabi_realloc is defined/expected by wit-bindgen
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

void exports_microquickjs_eval(microquickjs_string_t *code, microquickjs_string_t *ret) {
    if (ctx == NULL) {
        size_t mem_size = 16 << 20;
        uint8_t *mem_buf = malloc(mem_size);
        ctx = JS_NewContext(mem_buf, mem_size, &js_stdlib);
    }

    JSValue val = JS_Eval(ctx, (const char *)code->ptr, code->len, "<eval>", 0);

    const char *result_str = NULL;
    char *error_prefix = "";

    if (JS_IsException(val)) {
        JSValue err = JS_GetException(ctx);
        JSCStringBuf buf;
        result_str = JS_ToCString(ctx, err, &buf);
        error_prefix = "Error: ";
    } else {
        JSCStringBuf buf;
        result_str = JS_ToCString(ctx, val, &buf);
    }

    if (!result_str) result_str = "undefined";

    size_t prefix_len = strlen(error_prefix);
    size_t str_len = strlen(result_str);
    size_t total_len = prefix_len + str_len;

    uint8_t *out = cabi_realloc(NULL, 0, 1, total_len + 1);
    memcpy(out, error_prefix, prefix_len);
    memcpy(out + prefix_len, result_str, str_len);
    out[total_len] = '\0';

    ret->ptr = out;
    ret->len = total_len;
}
