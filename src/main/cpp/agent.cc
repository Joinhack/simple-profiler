#include "vm.h"
#include "profiler.h"
#include "controller.h"

static VM *vm;

AGENTEXPORT jint JNICALL Agent_OnLoad(JavaVM *jvm, char *options, void *reserved) {
    return VM::init(jvm);
}