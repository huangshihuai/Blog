/**
 * g++ server.cpp -o server -g3 -O0
 */
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <cstring>
#include <errno.h>
#include <cstdlib>
#include <iostream>
#include <errno.h>
#include <vector>
#include <pthread.h>
#include <atomic>

using namespace std;
#define IP "0.0.0.0"
#define PORT 8080
#define PROCESS_NUM 2
#define LISTEN_NUM 2   // 就绪队列数量

atomic<int> acceptNum(0);
int create_sock(const char *ip, int port);
void acceptConnect(const int listenfd, std::vector<int> &fd_list);
void *runThread(void *);

int main()
{
    vector<pthread_t> thids;
    pthread_t pid;
    for(int i = 0; i < PROCESS_NUM; i++)
    {
        int res = pthread_create(&pid, nullptr, runThread, nullptr);
        if (0 != res) {
            continue;
        }
        thids.push_back(pid);
        sleep(1);
    }
    do {
        if (acceptNum >= 20) {
            // 新增一个accept
            int res = pthread_create(&pid, nullptr, runThread, nullptr);
            if (0 == res) {
                thids.push_back(pid);
            }
            break;
        }
    } while(true);
    for (auto it = thids.begin(); it != thids.end(); ++it) {
        pthread_join(*it, nullptr);
    }
}

void *runThread(void *data) {
    int listenfd = create_sock(IP, PORT);
    if (listenfd < 0) {
        cout << "create_sock error: " << pthread_self() << endl;
        return 0;
    }
    std::vector<int> fd_list;
    acceptConnect(listenfd, fd_list);
    close(listenfd);
    cout << "close :" << listenfd << endl;
    return 0;
}

void acceptConnect(const int listenfd, std::vector<int> &fd_list)
{
    pthread_t pid = pthread_self();
    int fd = 0;
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    memset(&addr, 0, len);
    cout << "acceptConnect :" << pid << endl;
    while (true)
    {
        fd = accept(listenfd, (struct sockaddr *)&addr, &len);
        if (fd < 0) {
            cout << "process " << pid << "size: " << acceptNum << endl;
            if (errno != EAGAIN || errno != EWOULDBLOCK) {
                cout << "errno: " << errno << endl;
            }
            break;
        } else {
            auto state = acceptNum.fetch_add(1);
            fd_list.push_back(fd);
        }
        cout << "pid: " << pid << " size: " << acceptNum << " client: " << fd << endl;
    }
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
    int reuseport = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuseaddr, sizeof(reuseaddr));
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEPORT, (char *)&reuseport, sizeof(reuseport));

    struct sockaddr_in srv;
    srv.sin_family = AF_INET;
    srv.sin_addr.s_addr = inet_addr(ip);
    srv.sin_port = htons(port);
    int ret = bind(listenfd, (struct sockaddr*)&srv, sizeof(sockaddr));
    if (ret < 0)
    {
        close(listenfd);
        return -1;
    }
    ret = listen(listenfd, LISTEN_NUM);
    if (ret < 0)
    {
        return -1;
    }
    return listenfd;
}
