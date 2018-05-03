# epoll 新特性: EPOLLEXCLUSIVE
## 能力
```
    * 解决了鲸群问题 - yes(但并不完美)
```
* [EPOLLEXCLUSIVE-patch](https://github.com/torvalds/linux/commit/df0108c5da561c66c333bb46bfe3c1fc65905898)
## 问题？
```
    借这篇文章，将阐述2个问题
        * 不支持EPOLLEXCLUSIVE, EPOLL存在鲸群问题
        * 支持EPILLEXCLUSIVE, EPOLL存在哪些问题
            * EPOLLEXCLUSIVE 在配合ET和LT也可能存在鲸群问题
            * EPOLLEXCLUSIVE 在临界状态(accept)下是否能够完美处理？
```
## No Support EPOLLEXCLUSIVE
### LT is bad
```
    * 场景1:
        * 2个线程A and B, 同时等待事件(epoll_wait)
        * 当有新请求到达时(accept)
        * A,B 将被全部唤醒(不能够及时的处理事件,导致全部被唤醒)
        * A 线程 accept
        * B 线程 EAGAIN
    * 场景2:
        * 2个线程A and B, 同时等待事件(epoll_wait)
        * 当有行数据到达时
        * A,B 将被全部唤醒(可能)
        * A 线程 read data  |
                            | 导致数据包被2个线程所拥有
        * B 线程 read data  |
```
### 参考结果
![image](/Picture/no_support_exclusive.png)
```
1、场景1：
    *   process 3081202496 return from epoll_wait!  - A 被唤醒
    *   process 3081205504 return from epoll_wait!  - B 被唤醒
    *   accept:5 pid: 3081202496                    - A 有任务处理
    *   2个线程全部被唤醒, 只有pid=3081202496有任务-鲸群
2、场景2:
    *   process 3081202496 return from epoll_wait!  - A 被唤醒
    *   process 3081205504 return from epoll_wait!  - B 被唤醒
    *   3081205504 a                                - A 和 B 交错读取
    *   3081202496 a
    *   3081202496 a
    *   3081205504 a
    *   3081202496 a
    *   3081205504 a

3、测试方式：
    run server: ./no_support 0.0.0.0 8080 
    run client: telnet localhost 8080
        client 输入一组数据
```
# ET is bad
```
    * 场景1:
        * 2个线程A and B, 同时等待事件(epoll_wait)
        * 当有数据过来时(2048字节), 假设A被唤醒.
        * A开始读取,当我们认为读完数据时(2048),
        * 
    * 场景2:
        * 
```
# LT
# 临界值(accept)

## Support EPOLLEXCLUSIVE
### ET
### LT
