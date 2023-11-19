#ifndef _VM_H
#define _VM_H
#include "common.h"
#include "profiler.h"
#include "controller.h"

#define MAX_FRAME_DEEP 128


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

     static void handle_sigprof(int, siginfo_t*, void*);

private:
    static JavaVM *_jvm;
    static Profiler* _prof;
    static ControllerServ *_con_svr;
    static AsgcType _asgc;
};

#endif