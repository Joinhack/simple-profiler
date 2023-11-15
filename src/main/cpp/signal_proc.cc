#include <sys/time.h>
#include "common.h"
#include "signal_proc.h"

void SignalProc::install_sigprof_action(SigActionFn action) {
    struct sigaction sa;
    struct sigaction oldsa;
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = action;
    sa.sa_flags = SA_SIGINFO | SA_RESTART;
    sigaction(SIGPROF, &sa, &oldsa);
}

bool SignalProc::update_interval() {
    bool rs = update_interval(_intervals[_interval_index]);
    _interval_index = (_interval_index + 1) % MAX_SIGNAL_SIZE;
    return rs;
}

bool SignalProc::update_interval(int interval) {
    if (_current_interval == interval) {
        return true;
    }
    struct itimerval timer;
    timer.it_interval.tv_sec = interval/1000000000;
    timer.it_interval.tv_usec = (interval%1000000)%1000;
    timer.it_value = timer.it_interval;
    if (setitimer(ITIMER_PROF, &timer, nullptr) < 0) {
        log_error("ERROR: Set the timer error. \n");
        return false;
    }
    _current_interval = interval;
    return true;
}