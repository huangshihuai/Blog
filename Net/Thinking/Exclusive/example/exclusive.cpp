#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <cstring>
#include <wait.h>
#include <errno.h>
#include <sys/epoll.h>
#include <cstdlib>
#include <iostream>
#include <errno.h>
#include <netinet/tcp.h>
#include <netdb.h>

using namespace std;
#define PROCESS_NUM 3
#define MAXEVENTS 64

int create_sock(const char *ip, int port);
int create_epoll(uint64_t);
void test_epoll(int epollfd, int listenfd, struct epoll_event *events);
void acceptConnect(const int &epollfd, int listenfd);
int add_event(int epollfd, int fd, bool oneshot);
int mod_event(int epollfd, int fd, bool oneshot);

int main()
{
    const char *ip = "0.0.0.0";
    const int port = 8080;
    int listenfd = create_sock(ip, port);

    struct epoll_event event, *events;
    int epollfd = create_epoll(MAXEVENTS);
    int state = add_event(epollfd, listenfd, true);
    if (state < 0)
    {
        cout << "epoll_ctl add errno: " << errno << "\n msg: " << strerror(errno) << endl;
        goto jump_exit;
    }
    events = (struct epoll_event*)calloc(MAXEVENTS, sizeof(event));

    for(int i = 0; i < PROCESS_NUM; i++)
    {
        int pid = fork();
        if(pid == 0)
        {
            test_epoll(epollfd, listenfd, events);
            return 0;
        }
    }
    int status;
    wait(&status);
    jump_exit:
    close(listenfd);
    return 0;
}

int add_event(int epollfd, int fd, bool oneshot)
{
    struct epoll_event event;
    event.events = EPOLLIN | EPOLLET;
    if (oneshot)
    {
        event.events |= EPOLLEXCLUSIVE;
    }
    event.data.fd = fd;
    return epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
}

int mod_event(int epollfd, int fd, bool oneshot)
{
    struct epoll_event event;
    event.events = EPOLLIN | EPOLLET;
    if (oneshot)
    {
        event.events |= EPOLLONESHOT;
    }
    event.data.fd = fd;
    return epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event);
}

int del_event(int epollfd, int fd)
{
    struct epoll_event event;
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = fd;
    return epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event);
}

void test_epoll(int epollfd, int listenfd, struct epoll_event *events)
{
    pid_t pid = getpid();
    cout << "this pid: " << pid << endl;
    int size;
    do {
        size = epoll_wait(epollfd, events, MAXEVENTS, -1);
        cout << "process " << pid << " return from epoll_wait!" << endl;
        for (int i = 0; i < size; i++) {
            if (listenfd == events[i].data.fd) {
                acceptConnect(epollfd, listenfd);
                mod_event(epollfd, listenfd, true);
	        } else if (events[i].events & EPOLLIN) {
                cout << "pid: " << pid << "epoll error test epoll\n" << endl;
                continue;
            } else {
                cout << "events:" << events[i].events << endl;
                cout << "close fd: " << events[i].data.fd << endl;
                del_event(listenfd, events[i].data.fd);
                close(events[i].data.fd);
            }
        }
        cout << "process " << pid << " done!" << endl;
    } while(1);
}

void acceptConnect(const int &epollfd, int listenfd)
{
    uint32_t acceptCount = 0;
    static pid_t pid = getpid();
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    memset(&addr, 0, len);
    int fd = 0;
    while (true)
    {
        fd = accept(listenfd, (struct sockaddr *)&addr, &len);
        if (fd < 0) {
            if (acceptCount < 1) {
                cout << "sdsd";
                abort();
            }
            cout << "process " << pid << " accept count: " << acceptCount << endl;
            if (errno != EAGAIN || errno != EWOULDBLOCK) {
                cout << "errno: " << errno << endl;
            }
            return;
        } else {
            ++acceptCount;
            printf("process %d accept successed!\n", pid);
            add_event(epollfd, fd, true);
        }
    }
}

int create_epoll(uint64_t size)
{
    int epollfd = epoll_create(size);
    if (epollfd < 0)
    {
        perror("epoll_create");
        abort();
    }
    return epollfd;
}

int create_sock(const char *ip, int port)
{
    int listenfd = 0;
    listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenfd < 0)
    {
        return -1;
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
        return -1;
    }
    ret = listen(listenfd, 10);
    if (ret < 0)
    {
        return -1;
    }
    return listenfd;
}
