#include "vm.h"
#include "profiler.h"


void Profiler::AgentThreadRun(jvmtiEnv *jvmti_env, JNIEnv *jni_env, void *arg) {
    Profiler *profiler = static_cast<Profiler*>(arg);
    profiler->run();
}

bool Profiler::start(JNIEnv *jni_env) {
    bool run_flags = is_running();
    if (run_flags) 
        return true;
    jthread prof_thr = VM::new_jthread(jni_env, "Simple Profiler Thread");
    jvmtiError err = _jvmti_env->RunAgentThread(prof_thr, AgentThreadRun, this, JVMTI_THREAD_NORM_PRIORITY);
    if (err != JVMTI_ERROR_NONE) {
        log_error("ERROR: run agent thread error %d\n", err);
        return false;
    }
    running.store(true, std::memory_order_release);
    return true;
}

void Profiler::run() {
    while (is_running()) {

    }
}

void Profiler::update_sigprof_interval() {

}

void Profiler::stop() {
    running.store(false, std::memory_order_release);
}