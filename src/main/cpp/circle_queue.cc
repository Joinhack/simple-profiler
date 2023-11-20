#include <unistd.h>
#include "circle_queue.h"

bool CircleQueue::push(JVMPI_CallTrace &trace) {
    size_t curr_input, new_input;
    size_t ouput;
    do {
        curr_input = _input_idx.load(std::memory_order_relaxed);
        new_input = adivce(curr_input);
        if (new_input == _output_idx.load(std::memory_order_relaxed)) {
            return false;
        }
    } while (!_input_idx.compare_exchange_weak(curr_input, new_input, std::memory_order_relaxed));
    _handles[curr_input]._call_trace = trace;
    _handles[curr_input]._committed.store(true, std::memory_order_release) ;
    return true;
}

bool CircleQueue::pop() {
    size_t curr_output, new_output;
    size_t input;
    curr_output = _output_idx.load(std::memory_order_relaxed);
    input = _input_idx.load(std::memory_order_relaxed);
    if (curr_output == input) {
        return false;
    }
    while (!_handles[curr_output]._committed.load(std::memory_order_acquire)) {
        usleep(1);
    }
    CallTraceHandle &handle = _handles[curr_output];
    _handles[curr_output]._committed.store(false, std::memory_order_release);
    _output_idx.store(adivce(curr_output), std::memory_order_relaxed);
    return true;
}