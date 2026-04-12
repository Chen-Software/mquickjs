#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <inttypes.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <fcntl.h>
#include "cutils.h"
#include "mquickjs.h"

static JSValue js_print(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv)
{
    return JS_UNDEFINED;
}
static JSValue js_gc(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv)
{
    return JS_UNDEFINED;
}
static JSValue js_date_now(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv)
{
    return JS_UNDEFINED;
}
static JSValue js_performance_now(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv)
{
    return JS_UNDEFINED;
}
static JSValue js_load(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv)
{
    return JS_UNDEFINED;
}
static JSValue js_setTimeout(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv)
{
    return JS_UNDEFINED;
}
static JSValue js_clearTimeout(JSContext *ctx, JSValue *this_val, int argc, JSValue *argv)
{
    return JS_UNDEFINED;
}

#include "mqjs_stdlib.h"
