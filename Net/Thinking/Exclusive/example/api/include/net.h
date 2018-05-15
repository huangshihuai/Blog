#ifndef API_NET_H_
#define API_NET_H_
#include <sys/epoll.h>
#include <unistd.h>
#include "judgment.h"

namespace net {
int CreateSock(const char *ip, int port);
int Accept(int listenfd);
int ModEpoll(const int &&epollfd, int fd, int event_flags = EPOLLIN, int epoll_ctl_opt = EPOLL_CTL_ADD);
int Connect(int &clientfd, char *ip, int &&port);
int Close(int fd);
}
#endif // API_NET_H_