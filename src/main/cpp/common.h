#ifndef _GLOBAL_H
#define _GLOBAL_H
#include <jvmti.h>
#include <string>

#define AGENTEXPORT __attribute__((visibility("default"))) JNIEXPORT

#define COMMAND_ERROR 100

template<typename new_type, typename expression_type>
new_type bit_cast(const expression_type &expression) {
    new_type dest;
    //if the size is not equals, will be compiled error.
    typedef char TypeVerify[sizeof(new_type) == sizeof(expression_type)?1 : -1] 
        __attribute__((unused));
    memcpy(&dest, &expression, sizeof(dest));
    return dest;
}

void log_error(const char* format, ...);

#endif
