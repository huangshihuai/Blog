# epoll
```
epoll 对新手来说是一个麻烦事, 半知不解中夹杂着疑问.
我将介绍以下部分:
    * epoll 的线程安全
    * epoll 的一些flag(ET LT EPOLLONESHOT EPOLLEXCLUSIVE)
    * epoll 是否解决了惊群
```

## epoll线程安全吗?
```
首先它是非线程安全的, 可能有些同学该要问了: 从内核代码上看它是线程安全的(kernel 3.5以上)
    * 原因是:  有些内核版本的epoll它是不安全.
    * 其次, 我们从其他开源的网络框架上, 把对epoll的操作都集中在I/O线程上, 都是为了解决不同内核上线程安全
```
## [修复epoll丢失感兴趣文件描述符 - patch](https://lore.kernel.org/patchwork/patch/351273/)

## epoll 的一些flag(ET LT EPOLLONESHOT)

### EPOLLET[边缘触发]
```
怎么理解它?
    * 在每一次可读可写事件到达时, 将只会告知一次状态, 需要应用程序保证读写I/O的完整性. 如果没有全部读取完毕, 只能等待下次数据状态的改变/数据到达(很危险, 因为不清楚用户何在发送数据, 如果不发送将使该数据包不完整).
        * 判断读写完整性errno错误码为EAGAIN
优势:
    * 它性能最高, 因为维护的就绪句柄文件较少.
劣势:
    * 但容错性差, 开发难度大.
    * 避免多线程/多进程并发读写 - (当我们采用dispatch到work线程, 还未读取全部数据时, 远端数据到达, 触发一次状态改变.)
```

### EPOLLLT
```
怎么理解它?
    * 它始终将通知应用程序读写状态, 防止因为socket状态没处理完善而丢失文件句柄.
优势:
    * 开发要求较低, 容错性较大.
劣势:
    * 性能先对ET较差.
    * 避免多线程并发读写 - (当我们采用dispatch到work线程, 并且没有从epoll集合中移除该句柄, 将不断触发epoll_wait).
```

### EPOLLONESHOT
```
怎么理解它?
    * 每次事件都将从epoll集合中移除, 需要应用程序再次ctl到epoll中.
应用场景:
    * 可以将task派发到work线程中处理, 读写I/O和业务逻辑处理与I/O线程剥离.
优势:
    * 避免对同一个句柄并发读/写
劣势:
    * 多一次系统调用
```

## epoll 是否解决了惊群
```
    * 一次事件将唤醒多个等待该事件的线程或者进程
    * 已经解决惊群问题，但工程上还存在并发读写问题。
```

### EPOLLEXCLUSIVE
```
怎么理解它?
    * 将同一个fd共享在多个epoll集合中时, 它保证了原子唤醒, 也就是理论上不会出现惊群问题.
实际现象:
    * 惊群依然存在, 原因是EPOLLEXCLUSIVE与 ET或者LT 组合会导致惊群依然存在.
    * 每一次事件的产生都会使epoll被唤醒一次. 在ET模式下连续的信号将使epoll被多次唤醒, 在LT模式下事件的不及时处理将导致epoll被多次唤醒.
        * 后果: 导致对socket操作的并发问题, 导致cpu空转, 不能非常有效利用cpu.
```

### 如何解决惊群问题和并发问题
```
对于惊群问题建议: 避免在epoll中共享fd
    * 使用ET/LT + EPOLLONESHOT. 在事件到达时, 将从所属的epoll中移除fd. 代价: 每次需要将fd重新ctl到epoll中(多一次系统调用)
        * 开发要求较低, 容错性较高.
    * ET + lock-free. 这个方案存在并发问题, 需要保证读写顺序性.
```
