#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <cstring>
#include <wait.h>
#include <errno.h>
#include <cstdlib>
#include <iostream>
#include <errno.h>
#include <vector>

using namespace std;
#define PROCESS_NUM 2
#define LISTEN_NUM 10   // 就绪队列数量
// 连接数量
uint32_t acceptCount = 0;
uint32_t switchCount = 0;

int create_sock(const char *ip, int port);
void acceptConnect(const int listenfd, std::vector<int> &fd_list);

int main()
{
    const char *ip = "0.0.0.0";
    const int port = 8080;
    for(int i = 0; i < PROCESS_NUM; i++)
    {
        ++switchCount;
	    int pid = fork();
	    if (pid == 0) {
		    int listenfd = create_sock(ip, port);
            if (listenfd < 0) {
                goto jump_exit;
            }
            std::vector<int> fd_list;
            acceptConnect(listenfd, fd_list);
		    close(listenfd);
            cout << "close :" << listenfd << endl;
            while(true){sleep(100);}
            return 0;
	    }
    }
    int status;
    wait(&status);
    jump_exit:
    return 0;
}

void acceptConnect(const int listenfd, std::vector<int> &fd_list)
{
    pid_t pid = getpid();
    int fd = 0;
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    memset(&addr, 0, len);
    cout << "acceptConnect :" << pid << " acceeptCount:" << switchCount << endl;
    while (true)
    {
        fd = accept(listenfd, (struct sockaddr *)&addr, &len);
        if (fd < 0) {
            cout << "process " << pid << " accept count: " << acceptCount << " size: " << fd_list.size() << endl;
            if (errno != EAGAIN || errno != EWOULDBLOCK) {
                cout << "errno: " << errno << endl;
            }
            break;
        } else {
            ++acceptCount;
            fd_list.push_back(fd);
            close(fd);
        }
        cout << "pid: " << pid << " size: " << fd_list.size() << endl;
        if (switchCount % 2 && acceptCount == LISTEN_NUM) {
            sleep(2); //模拟大规模连接
            break;
        }
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
        return -1;
    }
    ret = listen(listenfd, LISTEN_NUM);
    if (ret < 0)
    {
        return -1;
    }
    return listenfd;
}
