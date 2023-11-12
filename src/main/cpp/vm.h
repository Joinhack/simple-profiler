#ifndef _VM_H
#define _VM_H
#include "common.h"
#include "profiler.h"
#include "controller.h"

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

class VM {
public:
    static int init(JavaVM *_jvm);

    static void setAsgc(AsgcType asgc) {
        _asgc = asgc;
    }

     static AsgcType getAsgc() {
        return _asgc;
     }

     static JNIEnv *get_jni_env();

     static jthread new_jthread(JNIEnv *jni_env, const char *thr_name);

     static void JNICALL VMInit(jvmtiEnv* jvmti, JNIEnv* jni, jthread thread);

private:
    static JavaVM *_jvm;
    static Profiler* _prof;
    static ControllerServ *_con_svr;
    static AsgcType _asgc;
};

#endif