#ifndef _PROFILER_H
#define _PROFILER_H
#include <atomic>
#include <jvmti.h>
#include "signal_proc.h"
#define DEFAULT_INTERVAL 1

class Profiler {
private:
    std::atomic<bool> running;
    jvmtiEnv *_jvmti_env;
    SignalProc *signal_proc;
    
    static void AgentThreadRun(jvmtiEnv *jvmti_env, JNIEnv *jni_env, void *arg);

    static void IntervalHandle();
public:
    Profiler(jvmtiEnv *jvmti_env): 
        running(false),_jvmti_env(jvmti_env) {
            signal_proc = new SignalProc(DEFAULT_INTERVAL, DEFAULT_INTERVAL);
    }

    bool is_running() {
        return running.load(std::memory_order_acquire);
    }

    void update_interval();

    bool start(JNIEnv *jni_env);

    void update_sigprof_interval();

    void stop();

    void run();
};

#endif