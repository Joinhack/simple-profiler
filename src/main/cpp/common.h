#ifndef _GLOBAL_H
#define _GLOBAL_H
#include <jvmti.h>
#include <string>

#define AGENTEXPORT __attribute__((visibility("default"))) JNIEXPORT

#define COMMAND_ERROR 100

void log_error(const char* format, ...);

#endif
