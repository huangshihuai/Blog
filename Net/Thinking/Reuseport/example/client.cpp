/**
 * g++ client.cpp  -pthread -o client -g3 -O0
 */
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <vector>
#include <atomic>
#include <mutex>

#define THR_NUM 10
#define IP "127.0.0.1"
#define PORT 8080
#define THR_RUN_SIZE 10

using namespace std;
atomic<int> connNum;
std::mutex g_print_mutex;
int create_socket(const char *ip, const int port);

void *runThread(void *) {
    pthread_t pid = pthread_self();
    vector<int> fds;
    for (int i = 0; i < THR_RUN_SIZE; i++) {
        int fd = create_socket(IP, PORT);
        if (fd <= 0) {
            cout << "connect error" << endl;
            continue;
        }
        connNum.fetch_add(1);
        fds.push_back(fd);
        // close(fd);
    }
    for (auto it = fds.begin(); it != fds.end(); ++it) {
        std::lock_guard<std::mutex> print_mutex(g_print_mutex);
        cout << "pid: " << pid << "connect fd: " << *it << endl;
    }
}

int main(void) {
    vector<pthread_t> thread;
    for (int i = 0; i < THR_NUM; i++) {
        pthread_t id;
        int err = pthread_create(&id, nullptr, runThread, nullptr);
        if (err != 0) {
            cout << "error number: " << err << endl;
            continue;
        }
        thread.push_back(id);
    }
    for (auto it = thread.begin(); it != thread.end(); it++) {
        pthread_join(*it, nullptr);
    }
    cout << "connect number: " << connNum << endl;
    while(true) {
        sleep(100);
    }
    return 0;
}

int create_socket(const char *ip, const int port) {
    int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (-1 == fd) {
        return fd;
    }
    struct sockaddr_in cli;
    cli.sin_family = AF_INET;
    cli.sin_addr.s_addr = inet_addr(ip);
    cli.sin_port = htons(port);
    int conn = connect(fd, (struct  sockaddr *)&cli, sizeof(cli));
    if (-1 == conn) {
        return conn;
    }
    return fd;
}
