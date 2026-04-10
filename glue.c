#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include "mquickjs.h"
#include "generated/microquickjs.h"

JSValue js_print(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv);
JSValue js_gc(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv);
JSValue js_date_now(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv);
JSValue js_performance_now(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv);
JSValue js_load(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv);
JSValue js_setTimeout(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv);
JSValue js_clearTimeout(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv);

#include "mqjs_stdlib.h"

void *cabi_realloc(void *ptr, size_t old_size, size_t align, size_t new_size);

static void *make_wasi_string(const char *src, size_t len) {
    if (!src) return NULL;
    uint8_t *out = (uint8_t *)cabi_realloc(NULL, 0, 1, len);
    if (!out) return NULL;
    memcpy(out, src, len);
    return out;
}

JSValue js_print(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv) {
    int i;
    JSValue v;
    for(i = 0; i < argc; i++) {
        if (i != 0) putchar(' ');
        v = argv[i];
        if (JS_IsString(ctx, v)) {
            JSCStringBuf buf;
            const char *str;
            size_t len;
            str = JS_ToCStringLen(ctx, &len, v, &buf);
            fwrite(str, 1, len, stdout);
        } else {
            JS_PrintValueF(ctx, argv[i], JS_DUMP_LONG);
        }
    }
    putchar('\n');
    return JS_UNDEFINED;
}

JSValue js_gc(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv) {
    JS_GC(ctx);
    return JS_UNDEFINED;
}

static int64_t get_time_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000 + (ts.tv_nsec / 1000000);
}

JSValue js_date_now(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return JS_NewInt64(ctx, (int64_t)tv.tv_sec * 1000 + (tv.tv_usec / 1000));
}

JSValue js_performance_now(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv) {
    return JS_NewInt64(ctx, get_time_ms());
}

JSValue js_load(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv) {
    return JS_ThrowInternalError(ctx, "load() not supported in WASI");
}

JSValue js_setTimeout(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv) {
    return JS_ThrowInternalError(ctx, "setTimeout() not supported in WASI");
}

JSValue js_clearTimeout(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv) {
    return JS_ThrowInternalError(ctx, "clearTimeout() not supported in WASI");
}

bool exports_microquickjs_eval(microquickjs_string_t *code, microquickjs_string_t *ok, microquickjs_string_t *err) {
    size_t mem_size = 1024 * 1024;
    uint8_t *mem_buf = malloc(mem_size);
    if (!mem_buf) {
        const char *msg = "Internal error: failed to allocate memory for JS context";
        err->len = strlen(msg);
        err->ptr = (uint8_t *)make_wasi_string(msg, err->len);
        return false;
    }
    JSContext *ctx = JS_NewContext(mem_buf, mem_size, &js_stdlib);
    if (!ctx) {
        free(mem_buf);
        const char *msg = "Internal error: failed to create JS context";
        err->len = strlen(msg);
        err->ptr = (uint8_t *)make_wasi_string(msg, err->len);
        return false;
    }
    JSValue val = JS_Eval(ctx, (const char *)code->ptr, code->len, "<eval>", JS_EVAL_RETVAL);
    if (JS_IsException(val)) {
        JSValue exception = JS_GetException(ctx);
        JSCStringBuf cstr_buf;
        size_t len;
        const char *exc_str = JS_ToCStringLen(ctx, &len, exception, &cstr_buf);
        err->len = len;
        err->ptr = (uint8_t *)make_wasi_string(exc_str, len);
        JS_FreeContext(ctx);
        free(mem_buf);
        return false;
    } else {
        JSCStringBuf cstr_buf;
        size_t len;
        const char *res_str = JS_ToCStringLen(ctx, &len, val, &cstr_buf);
        ok->len = len;
        ok->ptr = (uint8_t *)make_wasi_string(res_str, len);
        JS_FreeContext(ctx);
        free(mem_buf);
        return true;
    }
}
