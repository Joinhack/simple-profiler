#ifndef _CIRCLE_QUEUE_H
#define _CIRCLE_QUEUE_H
#include <atomic>
#include "common.h"

#define QUEUE_SIZE 1024

class CallTraceHandle {
private:
    JVMPI_CallTrace _call_trace;
    std::atomic<bool> _committed;
public:
    CallTraceHandle(JVMPI_CallTrace &call_trace): _committed(false) {
        _call_trace = call_trace;
    }

    CallTraceHandle(): _committed(false) {
    }

};

class CircleQueue {
private:
    CallTraceHandle _handles[QUEUE_SIZE];

    std::atomic<int> _input_idx = 0;
    
    std::atomic<int> _output_idx = 0;

    size_t adivce(size_t step);

    
public:
    CircleQueue() {
        
    }

    bool push(CallTraceHandle &handle);

    bool pop(CallTraceHandle &handle);
};
#endif //_CIRCLE_QUEUE_H

