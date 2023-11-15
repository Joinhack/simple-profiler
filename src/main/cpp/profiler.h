#ifndef _PROFILER_H
#define _PROFILER_H
#include <atomic>
#include <jvmti.h>
#include "signal_proc.h"
#define DEFAULT_MIN_INTERVAL 10
#define DEFAULT_MAX_INTERVAL 100

class Profiler {
private:
    std::atomic<bool> _running;
    jvmtiEnv *_jvmti;
    SignalProc *_signal_proc;
    
    static void AgentThreadRun(jvmtiEnv *jvmti_env, JNIEnv *jni_env, void *arg);

    static void IntervalHandle();
public:
    Profiler(jvmtiEnv *jvmti_env): 
        _running(false),_jvmti(jvmti_env) {
            _signal_proc = new SignalProc(DEFAULT_MIN_INTERVAL, DEFAULT_MAX_INTERVAL);
    }

    bool is_running() {
        return _running.load(std::memory_order_acquire);
    }

    void update_interval();

    bool start(JNIEnv *jni_env);

    void update_sigprof_interval();

    void stop();

    void run();

    void sleep(int period);
};

#endif