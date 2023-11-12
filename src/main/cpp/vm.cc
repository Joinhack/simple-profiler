#include <dlfcn.h>
#include "vm.h"

JavaVM* VM::_jvm;
Profiler* VM::_prof;
ControllerServ* VM::_con_svr;
AsgcType VM::_asgc;

int VM::init(JavaVM *jvm) {
    _jvm = jvm;
    void* symbol = dlsym(RTLD_DEFAULT, "AsyncGetCallTrace");
    AsgcType asgc = bit_cast<AsgcType>(symbol);
    setAsgc(asgc);

    jvmtiEnv *jvmti;
    
    jint rs = _jvm->GetEnv(reinterpret_cast<void**>(&jvmti), JVMTI_VERSION);
    if (rs != 0) {
        log_error("ERROR: JVMTI initialisation error %d", rs);
        return COMMAND_ERROR;
    }

    _prof = new Profiler(jvmti);
    _con_svr = new ControllerServ(jvmti, _prof);

    jvmtiEventCallbacks callbacks = {0};
    callbacks.VMInit = VMInit;
    jvmti->SetEventCallbacks(&callbacks, sizeof(callbacks));
    jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_VM_INIT, NULL);
    return 0;
}

void JNICALL VM::VMInit(jvmtiEnv* jvmti, JNIEnv* jni, jthread thread) {
    _con_svr->start(jni);
}

JNIEnv* VM::get_jni_env() {
    JNIEnv *jni_env = NULL;
    int getEnvStat = _jvm->GetEnv((void **) &jni_env, JNI_VERSION_1_6);
    // check for issues
    if (getEnvStat == JNI_EDETACHED || getEnvStat == JNI_EVERSION) {
        jni_env = NULL;
    }
    return jni_env;
}

jthread VM::new_jthread(JNIEnv *jni_env, const char *thr_name) {
    jclass thr_clz;
    jmethodID clz_mid;
    jobject thr_obj;
    thr_clz = jni_env->FindClass("java/lang/Thread");
    if (thr_clz == NULL) {
        log_error("WARNING: Thread class not found\n");
    }
    clz_mid = jni_env->GetMethodID(thr_clz, "<init>", "()V");
    if (clz_mid == NULL) {
        log_error("WARNING: Thread construct method not found\n");
    }
    thr_obj = jni_env->NewObject(thr_clz, clz_mid);
    if (thr_obj == NULL) {
        log_error("WARNING: Thread new object fail.\n");
    }
    if (jni_env != NULL) {
        jmethodID set_name = jni_env->GetMethodID(thr_clz, "setName", "(Ljava/lang/String;)V");
        jstring utf8name = jni_env->NewStringUTF(thr_name);
        jni_env->CallObjectMethod(thr_obj, set_name, utf8name);
    }
    return thr_obj;
}