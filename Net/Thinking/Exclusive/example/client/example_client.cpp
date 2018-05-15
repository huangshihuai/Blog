#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include "net.h"
#include "judgment.h"

using namespace std;
const char *data = "abcdefgh";

int main(int argc, char **argv) {
    if (argc != 5) {
        cerr << "请输入: ip 端口 执行次数\n";
        return -1;
    }
    char *ip = argv[1];
    int port = atoi(argv[2]);
    int count = atoi(argv[3]);
    size_t dataLen = strlen(data);
    NonNegative(port, "检查输入prot", return -1);
    NonNegative(count, "检查输入count", return -1);
    int clientfd;
    int ret = net::Connect(clientfd, ip, std::move(port));
    ssize_t sendLen = 0;
    if (0 != ret) {
        return -1;
    }
    do {
        sendLen = write(clientfd, (void*)(data), dataLen);
        if (sendLen == -1) {
            std::cerr << "wriet error, errno: " << errno << strerror(errno) << endl;
            break;
        }
        std::cout << "wriet size: " << sendLen << endl;
    } while(--count);
    sleep(20);
    net::Close(clientfd);
}