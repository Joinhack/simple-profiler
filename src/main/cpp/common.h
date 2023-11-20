#ifndef _GLOBAL_H
#define _GLOBAL_H
#include <jvmti.h>
#include <string>

typedef struct {
    jint lineno;
    jmethodID method_id;
} JVMPI_CallFrame;

typedef struct {
    // JNIEnv of the thread from which we grabbed the trace
    JNIEnv *env;
    // < 0 if the frame isn't walkable
    jint num_frames;
    // The frames, callee first.
    JVMPI_CallFrame *frames;
} JVMPI_CallTrace;

typedef void (*AsgcType)(JVMPI_CallTrace*, jint, void*);

#define AGENTEXPORT __attribute__((visibility("default"))) JNIEXPORT

#define COMMAND_ERROR 100

void log_error(const char* format, ...);

void log_trace(const char* format, ...);

#endif
