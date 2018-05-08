#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include "net.h"

namespace net {
int AddEpoll(const int &&epollfd, int fd, int event_flags, int epoll_ctl_opt) {
    struct epoll_event event;
    event.data.fd = fd;
    event.events = event_flags;
    int state = epoll_ctl(epollfd, epoll_ctl_opt, fd, &event);
    return state;
}

int Accept(int listenfd) {
    struct sockaddr in_addr;
    socklen_t in_len;
    in_len = sizeof(in_addr);
    return accept4(listenfd, &in_addr, &in_len, SOCK_CLOEXEC | SOCK_NONBLOCK);
}

int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    return ::connect(sockfd, addr, addrlen);
}

int CreateSock(const char *ip, int port)
{
    int listenfd = 0;
    listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenfd < 0)
    {
        return listenfd;
    }
    int reuseaddr = 1;
    int keepalive = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuseaddr, sizeof(reuseaddr));
    setsockopt(listenfd, SOL_SOCKET, SO_KEEPALIVE, (char *)&keepalive, sizeof(keepalive));

    unsigned long nonblocking = 1;
    ioctl(listenfd, FIONBIO, (void *)&nonblocking);

    struct sockaddr_in srv;
    srv.sin_family = AF_INET;
    srv.sin_addr.s_addr = inet_addr(ip);
    srv.sin_port = htons(port);
    int ret = bind(listenfd, (struct sockaddr*)&srv, sizeof(sockaddr));
    if (ret < 0)
    {
        return ret;
    }
    ret = listen(listenfd, 5);
    if (ret < 0)
    {
        return ret;
    }
    return listenfd;
}
}