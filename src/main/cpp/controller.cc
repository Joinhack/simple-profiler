#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "controller.h"
#include "vm.h"

void ControllerServ::ServThreadRun(jvmtiEnv* jvmti_env, JNIEnv* jni_env, void* arg) {
    ControllerServ *serv = static_cast<ControllerServ*>(arg);
    serv->run();
}

bool ControllerServ::start(JNIEnv *jni_env) {
    running.store(true, std::memory_order_relaxed);
    jthread thr = VM::new_jthread(jni_env, "Agent Controler Thread");
    jvmtiError error = _jvmti_env->RunAgentThread(thr, ControllerServ::ServThreadRun, this, JVMTI_THREAD_NORM_PRIORITY);
    if (error != JVMTI_ERROR_NONE) {
        log_error("ERROR: Run Controller Svr Agent Thread error \n");
        stop();
        return false;
    }
    return true;
}

void ControllerServ::stop() {
    running.store(false, std::memory_order_relaxed);   
}

class Socket {
public:
    int _fd;
    Socket(int sock): _fd(sock)  {}

    ~Socket() {
        close(_fd);
    }
};

bool ControllerServ::run() {
    int s_fd;
	struct sockaddr_in addr;	/* binary address */
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(5000);
    if ((s_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        log_error("ERROR: socket create error %d\n", s_fd);
        return false;
    }
    Socket svr_sock = s_fd;
    if (bind(svr_sock._fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        log_error("ERROR: bind error\n");
        return false;
    }
    if (listen(svr_sock._fd, 3) < 0) {
        log_error("ERROR: listen error\n");
        return false;
    }
    while(is_running()) {
        struct sockaddr peer_addr;
        socklen_t slen;
        int buf_len = 1024;
        char buf[buf_len];

        int n;
        int peer_fd = accept(svr_sock._fd, &peer_addr, &slen);
        if (peer_fd < 0) {
            log_error("ERROR: accept error\n");
            continue;
        }
        Socket peer_sock = peer_fd;
        while ((n = read(peer_sock._fd, buf, buf_len)) > 0) {
            n = n < buf_len? n: buf_len - 1;
            buf[n] = 0;
            if (strncmp(buf, "start", n)) {
                _profiler->start(VM::get_jni_env());
            } else if (strncmp(buf, "quit", n)) {
                break;
            }
        }
    }
    return true;
}