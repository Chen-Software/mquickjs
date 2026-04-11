#include <stddef.h>

int __imported_wasi_snapshot_preview1_args_sizes_get(size_t *argc, size_t *argv_buf_size) {
    if (argc) *argc = 0;
    if (argv_buf_size) *argv_buf_size = 0;
    return 0;
}

int __imported_wasi_snapshot_preview1_args_get(char **argv, char *argv_buf) {
    return 0;
}

int __imported_wasi_snapshot_preview1_fd_close(int fd) {
    return 0;
}

int __imported_wasi_snapshot_preview1_fd_seek(int fd, long long offset, int whence, long long *new_offset) {
    return 0;
}

int __imported_wasi_snapshot_preview1_fd_write(int fd, const void *iovs, size_t iovs_len, size_t *nwritten) {
    return 0;
}

void __imported_wasi_snapshot_preview1_proc_exit(int rval) {
}

int __imported_wasi_snapshot_preview1_environ_sizes_get(size_t *environ_count, size_t *environ_buf_size) {
    if (environ_count) *environ_count = 0;
    if (environ_buf_size) *environ_buf_size = 0;
    return 0;
}

int __imported_wasi_snapshot_preview1_environ_get(char **environ, char *environ_buf) {
    return 0;
}
