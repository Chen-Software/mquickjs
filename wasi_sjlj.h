#ifndef WASI_SJLJ_H
#define WASI_SJLJ_H
#include <stdlib.h>
typedef struct { int dummy; } wasi_jmp_buf_st;
typedef wasi_jmp_buf_st jmp_buf[1];
#define setjmp(env) (0)
#define longjmp(env, val) abort()
#endif
