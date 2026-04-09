#include "microquickjs.h"
#include "mquickjs.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void *cabi_realloc(void *ptr, size_t old_size, size_t align, size_t new_size);

static wasi_cli_stdout_own_output_stream_t s_stdout_stream = {0};
static bool s_stdout_initialized = false;

JSValue js_print(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv) {
    if (!s_stdout_initialized) {
        s_stdout_stream = wasi_cli_stdout_get_stdout();
        s_stdout_initialized = true;
    }
    wasi_io_streams_borrow_output_stream_t stream = wasi_io_streams_borrow_output_stream(s_stdout_stream);
    for(int i = 0; i < argc; i++) {
        if (i != 0) {
            uint8_t space = ' ';
            microquickjs_list_u8_t list = { &space, 1 };
            wasi_io_streams_stream_error_t err;
            wasi_io_streams_method_output_stream_blocking_write_and_flush(stream, &list, &err);
        }
        JSCStringBuf sbuf;
        size_t len;
        const char *str = JS_ToCStringLen(ctx, &len, argv[i], &sbuf);
        if (str) {
            microquickjs_list_u8_t list = { (uint8_t *)str, len };
            wasi_io_streams_stream_error_t err;
            wasi_io_streams_method_output_stream_blocking_write_and_flush(stream, &list, &err);
        }
    }
    uint8_t newline = '\n';
    microquickjs_list_u8_t list = { &newline, 1 };
    wasi_io_streams_stream_error_t err;
    wasi_io_streams_method_output_stream_blocking_write_and_flush(stream, &list, &err);
    return JS_UNDEFINED;
}

JSValue js_date_now(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv) {
    wasi_clocks_wall_clock_datetime_t dt;
    wasi_clocks_wall_clock_now(&dt);
    return JS_NewInt64(ctx, (int64_t)dt.seconds * 1000 + (dt.nanoseconds / 1000000));
}

JSValue js_performance_now(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv) {
    return js_date_now(ctx, this_val, argc, argv);
}

JSValue js_gc(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv) {
    JS_GC(ctx);
    return JS_UNDEFINED;
}

JSValue js_load(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv) {
    return JS_ThrowInternalError(ctx, "load() not supported in component");
}

JSValue js_setTimeout(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv) {
    return JS_ThrowInternalError(ctx, "setTimeout() not supported in component");
}

JSValue js_clearTimeout(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv) {
    return JS_UNDEFINED;
}

static JSContext *s_ctx = NULL;
static uint8_t s_mem_buf[1 << 20]; /* 1MB */

static void ensure_runtime(void) {
    if (s_ctx) return;
    extern const JSSTDLibraryDef js_stdlib;
    s_ctx = JS_NewContext(s_mem_buf, sizeof(s_mem_buf), &js_stdlib);
}

static char *make_wasi_string(const char *src) {
    if (!src) return NULL;
    size_t len = strlen(src);
    char *out = (char *)cabi_realloc(NULL, 0, 1, len);
    if (!out) return NULL;
    memcpy(out, src, len);
    return out;
}

bool exports_microquickjs_eval(microquickjs_string_t *code, microquickjs_string_t *ret, microquickjs_string_t *err) {
    ensure_runtime();
    char *src = malloc(code->len + 1);
    memcpy(src, code->ptr, code->len);
    src[code->len] = '\0';
    JSValue val = JS_Eval(s_ctx, src, code->len, "<eval>", JS_EVAL_RETVAL);
    free(src);
    if (JS_IsException(val)) {
        JSValue exc = JS_GetException(s_ctx);
        const char *result_cstr;
        JSCStringBuf sbuf;
        result_cstr = JS_ToCString(s_ctx, exc, &sbuf);
        err->ptr = (uint8_t *)make_wasi_string(result_cstr);
        err->len = strlen(result_cstr);
        return false;
    }
    const char *result_cstr;
    JSCStringBuf sbuf;
    result_cstr = JS_ToCString(s_ctx, val, &sbuf);
    ret->ptr = (uint8_t *)make_wasi_string(result_cstr);
    ret->len = strlen(result_cstr);
    return true;
}

bool exports_wasi_cli_run_run(void) {
    return true;
}
