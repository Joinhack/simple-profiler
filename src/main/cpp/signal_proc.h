#ifndef _SIGNAL_PROC_H
#define _SIGNAL_PROC_H
#include <array>
#include <signal.h>

#define MAX_SIGNAL_SIZE 1024



class SignalProc {
private:
    std::array<int, MAX_SIGNAL_SIZE>  _intervals;
    int _current_interval;
    int _interval_index;
public:
    typedef void(*SigActionFn)(int, siginfo_t*, void*);
    SignalProc() = delete;
    
    SignalProc(SignalProc &&f) = delete;

    SignalProc(SignalProc &f) = default;

    SignalProc(const int min_interval, const int max_interval) {
        int range = max_interval - min_interval + 1;
        for (auto iter = _intervals.begin(); iter != _intervals.end(); iter++) {
            *iter = min_interval + random()%range;
        }
    }

    void update_interval();

    void install_action(int signo, SigActionFn action);
    
};
#endif //_SIGNAL_PROC_H