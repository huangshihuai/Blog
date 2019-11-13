# 信号
```
非正常的操作, 会使操作系统发送信号, 如果没有处理该信号可能导致进程被kill.
例如: 当socket不可写时, 我们继续写本socket, 将使系统发送信号, 该信号会导致本进程被kill.
```

## code
```
void NetServer::installSignal() {
    signal(SIGPIPE, SIG_IGN); // socket/pip read/write failed
    signal(SIGCHLD, SIG_IGN); // child process was exit, will notification parent process.
    signal(SIGTSTP, SIG_IGN); // receive signal will stop the process
    signal(SIGTTOU, SIG_IGN); // from terminal
    signal(SIGTTIN, SIG_IGN); // from terminal
    signal(SIGHUP,  SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGURG,  SIG_IGN); // URG
    signal(SIGTERM, [](int sign) {
        switch(sign) {
            case SIGHUP:
            case SIGTERM:
                g_quit = true;
                break;
        }
    });
}
```
