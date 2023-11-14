#include <sys/types.h>
#include <iostream>
#ifdef WINDOWS
#include <windows.h>
#else
#include <unistd.h>
#endif
#include "vm.h"
#include "profiler.h"

const uint STATUS_CHECK_PERIOD = 100;
const uint MILLIS_IN_MICRO = 1000;

void sleep_for_millis(uint period) {
#ifdef WINDOWS
    Sleep(period);
#else
    usleep(period * MILLIS_IN_MICRO);
#endif
}

void Profiler::AgentThreadRun(jvmtiEnv *jvmti_env, JNIEnv *jni_env, void *arg) {
    Profiler *profiler = static_cast<Profiler*>(arg);
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGPROF);
    if (pthread_sigmask(SIG_BLOCK, &mask , nullptr)) {
        log_error("ERROR: error block thread SIGPROF\n");
    }
    profiler->run();
}

void Profiler::sleep(int period) {
    const int loop = period / STATUS_CHECK_PERIOD;
    const int remainder = period % STATUS_CHECK_PERIOD;
    int count = 0;
    while (count < loop && is_running()) {
        sleep_for_millis(STATUS_CHECK_PERIOD);
    }

    if (remainder > 0 && is_running()) {
        sleep_for_millis(STATUS_CHECK_PERIOD);
    }
}

bool Profiler::start(JNIEnv *jni_env) {
    bool run_flags = is_running();
    if (run_flags) 
        return true;
    _signal_proc->install_action(SIGPROF, VM::handle_sigprof);
    jthread prof_thr = VM::new_jthread(jni_env, "Simple Profiler Thread");
    jvmtiError err = _jvmti->RunAgentThread(prof_thr, AgentThreadRun, this, JVMTI_THREAD_NORM_PRIORITY);
    if (err != JVMTI_ERROR_NONE) {
        log_error("ERROR: run agent thread error %d\n", err);
        return false;
    }
    _running.store(true, std::memory_order_release);
    return true;
}

void Profiler::run() {
    std::cout << "MESSAGE: start profile" << std::endl;
    while (is_running()) {
        //TODO! record.
        _signal_proc->update_interval();
        sleep(1);
    }
}

void Profiler::stop() {
    _running.store(false, std::memory_order_release);
}