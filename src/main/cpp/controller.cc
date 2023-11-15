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
private:
    int _fd;
public:
    int fd() { 
        return _fd;
    }

    int bind(struct sockaddr* addr, size_t len) {
        return ::bind(_fd, addr, len);
    }

    int read(char *buf, size_t len) {
        return ::read(_fd, buf, len);
    }

    int listen(int backlog) {
        return ::listen(_fd, backlog);
    }

    int accept(struct sockaddr *peer_addr, socklen_t * len) {
        return ::accept(_fd, peer_addr, len);
    }

    int set_reuse() {
        int optval = 1;
        return setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    }

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
    if (svr_sock.set_reuse() < 0)  {
        log_error("ERROR: set socket reuse error\n");
        return false;
    }
    if (svr_sock.bind((struct sockaddr*)&addr, sizeof(addr)) < 0) {
        log_error("ERROR: bind error\n");
        return false;
    }
    if (svr_sock.listen(3) < 0) {
        log_error("ERROR: listen error\n");
        return false;
    }
    while(is_running()) {
        struct sockaddr peer_addr;
        socklen_t slen;
        int buf_len = 1024;
        char buf[buf_len];

        int n;
        int peer_fd = svr_sock.accept(&peer_addr, &slen);
        if (peer_fd < 0) {
            log_error("ERROR: accept error\n");
            continue;
        }
        Socket peer_sock = peer_fd;
        while ((n = peer_sock.read(buf, buf_len)) > 0) {
            n = n < buf_len? n: buf_len - 1;
            buf[n] = 0;
            if (strnstr(buf, "start", n) == buf) {
                _profiler->start(VM::get_jni_env());
            } else if (strnstr(buf, "quit", n) == buf) {
                break;
            } else if (strnstr(buf, "stop", n) == buf) {
                _profiler->stop();
            }
        }
    }
    return true;
}