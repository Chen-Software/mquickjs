#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "generated/microquickjs.h"

int main(int argc, char **argv) {
    if (argc < 2) return 0;
    microquickjs_string_t code;
    code.ptr = (uint8_t*)argv[1];
    code.len = strlen(argv[1]);
    microquickjs_string_t ok, err;
    if (exports_microquickjs_eval(&code, &ok, &err)) {
        printf("%.*s\n", (int)ok.len, ok.ptr);
        microquickjs_string_free(&ok);
    } else {
        fprintf(stderr, "JS Error: %.*s\n", (int)err.len, err.ptr);
        microquickjs_string_free(&err);
        return 1;
    }
    return 0;
}
