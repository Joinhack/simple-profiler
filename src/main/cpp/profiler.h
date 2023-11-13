#ifndef _PROFILER_H
#define _PROFILER_H
#include <atomic>
#include <jvmti.h>

class Profiler {
private:
    std::atomic<bool> running;
    jvmtiEnv *_jvmti_env;
    static void AgentThreadRun(jvmtiEnv *jvmti_env, JNIEnv *jni_env, void *arg);
public:
    Profiler(jvmtiEnv *jvmti_env): 
        running(false),_jvmti_env(jvmti_env) {
    }

    bool is_running() {
        return running.load(std::memory_order_acquire);
    }

    bool start(JNIEnv *jni_env);

    void stop();

    void run();
};

#endif