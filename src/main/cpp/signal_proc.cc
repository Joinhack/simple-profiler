#include "signal_proc.h"

void SignalProc::install_action(int signo, SigActionFn action) {
    struct sigaction sa;
    struct sigaction oldsa;
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = action;
    sa.sa_flags = SA_SIGINFO | SA_RESTART;
    sigaction(signo, &sa, &oldsa);
}

void SignalProc::update_interval() {
    int interval = _intervals[_interval_index];
    if (_current_interval == interval) {
        
    }
    _interval_index = (_interval_index + 1) % MAX_SIGNAL_SIZE;
    _current_interval = interval;
}