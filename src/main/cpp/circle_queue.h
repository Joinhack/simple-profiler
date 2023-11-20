#ifndef _CIRCLE_QUEUE_H
#define _CIRCLE_QUEUE_H
#include <atomic>
#include "common.h"

#define QUEUE_SIZE 1024
#define MAX_FRAMES_SIZE 1024

class CallTraceHandle {
protected:
    JVMPI_CallTrace _call_trace;
    std::atomic<bool> _committed;
public:
    friend class CircleQueue;
    CallTraceHandle(JVMPI_CallTrace &call_trace): _committed(false) {
        _call_trace = call_trace;
    }

    CallTraceHandle(): _committed(false) {
    }

};

class CircleQueue {
private:
    CallTraceHandle _handles[QUEUE_SIZE];
    JVMPI_CallFrame *_frames[MAX_FRAMES_SIZE];

    std::atomic<size_t> _input_idx;
    
    std::atomic<size_t> _output_idx;

    size_t adivce(size_t step) {
        return (step + 1) % QUEUE_SIZE;
    }

public:
    
    CircleQueue(): _input_idx(0), _output_idx(0) {
        for (size_t i = 0; i < QUEUE_SIZE; i++) {
            _frames[i] = new JVMPI_CallFrame[MAX_FRAMES_SIZE]();
        }
        _input_idx = 0;
    }
    
    bool push(JVMPI_CallTrace &trace);

    bool pop();
};
#endif //_CIRCLE_QUEUE_H

