#include "common.h"

void log_error(const char *__restrict format, ...) {
    va_list arg;
    va_start(arg, format);
    vfprintf(stderr, format, arg);
    va_end(arg);
}

void log_trace(const char *__restrict format, ...) {
    va_list arg;
    va_start(arg, format);
    vfprintf(stdout, format, arg);
    va_end(arg);
}

