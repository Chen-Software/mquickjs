#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "generated/microquickjs.h"
#ifdef TEST_RUNNER
void __component_type_object_force_link_microquickjs(void) {}
#endif
int main(int argc, char **argv) {
    if (argc < 2) { printf("Usage: test_eval <code>\n"); return 1; }
    microquickjs_string_t code;
    code.ptr = (uint8_t *)argv[1];
    code.len = strlen(argv[1]);
    microquickjs_string_t ok, err;
    if (exports_microquickjs_eval(&code, &ok, &err)) {
        printf("OK: %.*s\n", (int)ok.len, ok.ptr);
    } else {
        printf("ERR: %.*s\n", (int)err.len, err.ptr);
    }
    return 0;
}
