/////////////////////////////
// g++ old.cpp -pthread -g3 -O0
// ./a.out 0.0.0.0 8080 2
////////////////////////////////
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
#include <errno.h>
#include <pthread.h>
#include <iostream>
#include <vector>
#include <utility>
#include "net.h"

using namespace std;
using namespace net;
#define PROCESS_NUM 2
#define MAXEVENTS 64

struct threadData;

void *TestEpoll(void *);

struct threadData {
    int epollfd;
    int listenfd;
    struct epoll_event *events;
    threadData() : epollfd(-1), listenfd(-1), events(nullptr) {}
};

#define PRINT_STRERRNO() do { \
    std::cout << "str err: " << strerror(errno) << endl; \
} while(false)

int main(int argc, char **argv)
{
    if (4 != argc) {
        cerr << "please input ip、port、thread_num\n";
        return -1;
    }
    const char *ip = argv[1];
    int port = atoi(argv[2]);
    int threadnum = atoi(argv[3]);
    if (threadnum < 0) {
        threadnum = PROCESS_NUM;
    }
    threadData data;
    do {
        data.listenfd = CreateSock(ip, port);
        if (data.listenfd < 0)
        {
            cerr << "create sock errno: " << errno << "\n msg: " << strerror(errno) << endl;
            break;
        }

        struct epoll_event event;
        data.epollfd = epoll_create(MAXEVENTS);
        if (data.epollfd <= 0) {
            cerr << "create epoll error errno:" << errno << endl;
            break;
        }
        event.data.fd = data.listenfd;
        event.events = EPOLLIN | EPOLLET;
        int state = epoll_ctl(data.epollfd, EPOLL_CTL_ADD, data.listenfd, &event);
        if (state != 0) {
            cerr << "epoll ctl error, errno: " << errno << endl;
            break;
        }
        data.events = (struct epoll_event*)malloc(MAXEVENTS * sizeof(event));

        std::vector<pthread_t> threads;
        for(int i = 0; i < threadnum - 1; i++)
        {
            pthread_t pid;
            int &&res = pthread_create(&pid, nullptr, TestEpoll, (void *)&data);
            if (0 == res) {
                threads.push_back(pid);
            }
        }
        TestEpoll((void*)&data);
        for (auto pid = threads.begin(); pid != threads.end(); ++pid) {
            pthread_join(*pid, nullptr);
        }
    } while(false);
    if (data.listenfd > 0) {
        close(data.listenfd);
    }
    if (data.epollfd > 0) {
        close(data.epollfd);
    }
    if (nullptr != data.events) {
        free(data.events);
        data.events = nullptr;
    }
    return 0;
}

void *TestEpoll(void *data)
{
    pthread_t pid = pthread_self();;
    cout << "this pthread: " << pid << endl;
    if (nullptr == data) {
        cerr << "data is nullptr \n";
        return nullptr;
    }
    threadData *tData = static_cast<threadData *>(data);
    int size;
    do {
        size = epoll_wait(tData->epollfd, tData->events, MAXEVENTS, -1);
        cout << "process " << pid << " return from epoll_wait!\n";
        for (int i = 0; i < size; i++) {
            if ((tData->events[i].events & EPOLLERR) || (tData->events[i].events & EPOLLHUP | EPOLLERR)) {
                fprintf (stderr, "epoll error\n");
                close (tData->events[i].data.fd);
                continue;
            } else if (tData->events[i].events &  EPOLLIN) {
                if (tData->listenfd == tData->events[i].data.fd) {
                    while (true) {
                        int fd = Accept(tData->listenfd);
                        if (fd == -1 && EAGAIN == errno) {
                            break;
                        }
                        int state = ModEpoll(std::move(tData->epollfd), fd, EPOLLIN | EPOLLET | EPOLLHUP | EPOLLERR);
                        if (state != 0) {
                            PRINT_STRERRNO();
                            cerr << "epoll ctl error, fd: " << fd << " pid: " << pid << endl;
                            continue;
                        }
                        cout << "accept:" << fd << " pid: " << pid << endl;
                    }
                } else {
                    char c;
                    int state;
                    do {
                        state = read(tData->events[i].data.fd, &c, sizeof(c));
                        if (state <= 0 && (EAGAIN == errno || EWOULDBLOCK == errno)) {
                            net::Close(tData->events[i].data.fd);
                            ModEpoll(std::move(tData->epollfd), tData->events[i].data.fd, tData->events[i].events, EPOLL_CTL_DEL);
                            break;
                        }
                        cout << pid << " " << c << endl;
                        sleep(1); // 比较极端的场景, 用减缓时间来模拟
                    } while(true);
                }
            }
        }
    } while(1);
}