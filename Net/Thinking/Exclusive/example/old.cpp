#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <cstring>
#include <errno.h>
#include <sys/epoll.h>
#include <cstdlib>
#include <pthread.h>
#include <iostream>

using namespace std;
#define PROCESS_NUM 10
#define MAXEVENTS 64

int create_sock(const char *ip, int port);
int create_epoll(uint64_t);
void test_epoll(int epollfd, int listenfd, struct epoll_event *events);
void test_accept(int listenfd);

int main(int argc, char **argv)
{
    if (3 != argc) {
        cerr << "please input ip and port"
        return -1;
    }
    const char *ip = argv[1];
    int port = atoi(argv[2]);
    int listenfd = create_sock(ip, port);
    if (listenfd < 0)
    {
        cerr << "create sock errno: " << errno << "\n msg: " << strerror(errno) << endl;
        return -1;
    }

    struct epoll_event event, *events;
    int epollfd = create_epoll(MAXEVENTS);
    event.data.fd = listenfd;
    event.events = EPOLLIN;
    int state = epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &event);
    events = (struct epoll_event*)calloc(MAXEVENTS, sizeof(event));

    std::vector<pthread_t> threads;
    for(int i = 0; i < PROCESS_NUM; i++)
    {
        pthread_t pid;
        int &&res = pthread_create(&pid, nullptr, test_epoll, nullptr);
        if (0 == res) {
            threads.push_back(pid);
        }
    }
    for (auto pid = threads.begin(); pid != threads.end(); ++pid) {
        pthread_join(*pid);
    }
    close(listenfd);
    return 0;
}

void test_epoll(int epollfd, int listenfd, struct epoll_event *events)
{
    pid_t pid = getpid();
    cout << "this pid: " << pid << endl;
    int size;
    do {
        size = epoll_wait(epollfd, events, MAXEVENTS, -1);
        printf("process %d return from epoll_wait!\n", pid);
        for (int i = 0; i < size; i++) {
            if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) || (!(events[i].events & EPOLLIN)))
            {
                fprintf (stderr, "epoll error\n");
                close (events[i].data.fd);
                continue;
            }
            else if (listenfd == events[i].data.fd)
            {
                struct sockaddr in_addr;
                socklen_t in_len;
                int infd;
                in_len = sizeof in_addr;
                infd = accept(listenfd, &in_addr, &in_len);
                if (infd == -1)
                {
                    printf("process %d accept failed!\n", pid);
                    continue;
                }
                printf("process %d accept successed!\n", pid);
                close(infd);
            }
        }
    } while(1);
}

int create_epoll(uint64_t size)
{
    int epollfd = epoll_create(size);
    if (epollfd < 0)
    {
        return -1;
    }
    return epollfd;
}

int create_sock(const char *ip, int port)
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
