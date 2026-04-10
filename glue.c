#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "mquickjs.h"
#include "generated/microquickjs.h"

JSValue js_print(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv) {
    for (int i = 0; i < argc; i++) {
        JSCStringBuf buf;
        const char *str = JS_ToCString(ctx, argv[i], &buf);
        if (str) {
            printf("%s%s", i > 0 ? " " : "", str);
        }
    }
    printf("\n");
    return JS_UNDEFINED;
}

JSValue js_performance_now(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv) {
    return JS_NewFloat64(ctx, 0.0);
}

JSValue js_date_now(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv) {
    return JS_NewFloat64(ctx, 0.0);
}

JSValue js_gc(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv) {
    JS_GC(ctx);
    return JS_UNDEFINED;
}

JSValue js_load(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv) {
    return JS_ThrowTypeError(ctx, "load() is not supported in this WASI component");
}

JSValue js_setTimeout(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv) {
    return JS_ThrowTypeError(ctx, "setTimeout() is not supported in this WASI component");
}

JSValue js_clearTimeout(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv) {
    return JS_UNDEFINED;
}

#define JS_CLASS_COUNT JS_CLASS_USER
#include "mqjs_stdlib.h"

#define JS_HEAP_SIZE (1024 * 1024)
static uint8_t js_heap[JS_HEAP_SIZE];

bool exports_microquickjs_eval(microquickjs_string_t *code_ptr, microquickjs_string_t *ret, microquickjs_string_t *err) {
    JSContext *ctx;
    JSValue result;

    ctx = JS_NewContext(js_heap, JS_HEAP_SIZE, &js_stdlib);
    if (!ctx) {
        microquickjs_string_dup(err, "Internal error: failed to create JS context");
        return false;
    }

    result = JS_Eval(ctx, (const char *)code_ptr->ptr, code_ptr->len, "<stdin>", JS_EVAL_RETVAL);

    if (JS_IsException(result)) {
        JSValue exception = JS_GetException(ctx);
        JSCStringBuf buf;
        const char *exc_str = JS_ToCString(ctx, exception, &buf);
        microquickjs_string_dup(err, exc_str ? exc_str : "Unknown error");
        JS_FreeContext(ctx);
        return false;
    }

    JSCStringBuf buf;
    const char *result_str = JS_ToCString(ctx, result, &buf);
    if (!result_str) {
        microquickjs_string_dup(err, "Internal error: failed to convert result to string");
        JS_FreeContext(ctx);
        return false;
    }

    microquickjs_string_dup(ret, result_str);
    JS_FreeContext(ctx);
    return true;
}
