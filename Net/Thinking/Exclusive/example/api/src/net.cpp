#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include "net.h"

namespace net {
int ModEpoll(const int &&epollfd, int fd, int event_flags, int epoll_ctl_opt) {
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

int Connect(int &clientfd, char *ip, int &&port) {
    int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    NonNegative(fd, "create socket error", return clientfd);
    struct sockaddr_in clientAddr;
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons(port);
    clientAddr.sin_addr.s_addr = inet_addr(ip);
    int ret = ::connect(fd, (struct sockaddr *)&clientAddr, sizeof(struct sockaddr));
    if (0 == ret) {
        clientfd = fd;
    }
    return ret;
}

int Close(int fd) {
    return ::close(fd);
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