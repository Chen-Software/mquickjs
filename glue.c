#include "microquickjs.h"
#include "mquickjs.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern const JSSTDLibraryDef js_stdlib;
/* Forward declaration of canonical ABI allocator */
void *cabi_realloc(void *ptr, size_t old_size, size_t align, size_t new_size);

static JSContext *s_ctx = NULL;
static uint8_t *s_mem_buf = NULL;
#define MEM_SIZE (16 << 20)

static void ensure_runtime(void) {
    if (s_ctx) return;
    s_mem_buf = malloc(MEM_SIZE);
    if (!s_mem_buf) abort();
    s_ctx = JS_NewContext(s_mem_buf, MEM_SIZE, &js_stdlib);
    if (!s_ctx) abort();
}

void microquickjs_eval(microquickjs_string_t *code, microquickjs_string_t *ret) {
    ensure_runtime();

    size_t src_len = code->len;
    char *src = malloc(src_len + 1);
    if (!src) abort();
    memcpy(src, code->ptr, src_len);
    src[src_len] = '\0';

    JSValue val = JS_Eval(s_ctx, src, src_len, "<eval>", JS_EVAL_RETVAL);
    free(src);

    JSCStringBuf sbuf;
    if (JS_IsException(val)) {
        JSValue exc = JS_GetException(s_ctx);
        const char *result_cstr = JS_ToCString(s_ctx, exc, &sbuf);
        if (!result_cstr) result_cstr = "Unknown error";

        size_t elen = strlen(result_cstr) + 8; /* "Error: " + NUL */
        char *ebuf = cabi_realloc(NULL, 0, 1, elen);
        if (!ebuf) abort();
        snprintf(ebuf, elen, "Error: %s", result_cstr);

        ret->ptr = (uint8_t *)ebuf;
        ret->len = strlen(ebuf);
        return;
    }

    const char *result_cstr = JS_ToCString(s_ctx, val, &sbuf);
    if (!result_cstr) result_cstr = "";
    size_t len = strlen(result_cstr);
    char *out = cabi_realloc(NULL, 0, 1, len + 1);
    if (!out) abort();
    memcpy(out, result_cstr, len + 1);

    ret->ptr = (uint8_t *)out;
    ret->len = len;
}
