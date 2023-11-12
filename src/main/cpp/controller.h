#ifndef _CONTROLLER_H
#define _CONTROLLER_H
#include <atomic>
#include "profiler.h"

class ControllerServ {
private:
    Profiler *_profiler;
    jvmtiEnv *_jvmti_env;
    std::atomic<bool> running;
    bool run();
    static void ServThreadRun(jvmtiEnv* jvmti_env, JNIEnv* jni_env, void* arg);
public:
    ControllerServ(jvmtiEnv *jvmti_env, Profiler *profiler): 
        _jvmti_env(jvmti_env), _profiler(profiler) {}
    
    bool start(JNIEnv *jni_env);

    void stop();

    bool is_running() {
        return running.load(std::memory_order_relaxed);
    }
};
#endif 