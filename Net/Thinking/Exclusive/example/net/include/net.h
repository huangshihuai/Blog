#ifndef __NET_H_
#define __NET_H_
#include <sys/epoll.h>

namespace net {
int CreateSock(const char *ip, int port);
int Accept(int listenfd);
int AddEpoll(const int &&epollfd, int fd, int event_flags = EPOLLIN, int epoll_ctl_opt = EPOLL_CTL_ADD);
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
}
#endif // __NET_H_