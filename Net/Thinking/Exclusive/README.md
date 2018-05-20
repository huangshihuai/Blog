# epoll 新特性: EPOLLEXCLUSIVE
## 能力
```
* 解决了鲸群问题 - yes(但还是存在鲸群问题)
    - kernel能够保证epoll_wait是原子性(仅唤醒其中一个), 为什么还存在鲸群?
        * 仅讨论EPOLLEXCLUSIVE将是毫无意义的, 我们将结合EPOLL其它flag(例如ET和LT)
        * 众所周知, ET仅通知一次, LT将不断的通知(直到事件完全被处理)
        * epoll flage: EPOLLEXCLUSIVE + (ET or LT),  它将是不安全的, 场景如下:
            1) 事件不及时处理时
                * LT不断触发事件, 导致其他epoll_wait也被唤醒(每次触发事件,将唤醒其中一个)
                    - 被唤醒的线程无事可做
                    - 数据分裂: 多线程读写同一个文件描述
                * ET 能够正常处理(仅触发一次)
            2) 同一个fd不断有事件到达
                * LT不断触发事件, 导致其他epoll_wait也被唤醒(每次触发事件,将唤醒其中一个)
                    - 被唤醒的线程无事可做
                    - 数据分裂: 多线程读写同一个文件描述
                * ET不断触发事件, 导致其他epoll_wait也被唤醒(每次触发事件,将唤醒其中一个)
                    - 被唤醒的线程无事可做
                    - 数据分裂: 多线程读写同一个文件描述
```
* [EPOLLEXCLUSIVE-补丁](https://github.com/torvalds/linux/commit/df0108c5da561c66c333bb46bfe3c1fc65905898)

## 使用和不适用EPOLLEXCLUSIVE的区别是什么?
```
先不写,以免误导.
先研究研究...
``

## 该如何解决?
```
* 事件到达时移除epoll事件集,到需要读写时在加入epoll事件集, 多一些api调用
* 工程上的保证.
    - 
```

## 论证&问题
```
* 不支持EPOLLEXCLUSIVE, EPOLL存在鲸群问题
* 支持EPILLEXCLUSIVE, EPOLL也存在鲸群问题
* 多线程读写同一个fd
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
### 参考结
![image](/Picture/no_support_exclusive_lt.png)
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
    run server: output/bin/example_no_support_lt 0.0.0.0 8080 2
    run client: output/bin/example_no_support_client 0.0.0.0 8080 2
        server 验证accept和读写顺序: ip 端口 线程数量
            * server 通过sleep来模拟大规模请求,从而保证其他epoll_wait能够被唤醒
        client 发送数据: ip 端口 写次数
            * 多次写入是为了重制event状态
```
### ET is bad
```
    * 场景1(数据分割):
        * 2个线程A and B, 同时等待事件(epoll_wait)
        * 当有数据过来时(2048字节), 假设A被唤醒.
        * A开始读取数据(有几种情况)
            * A 读取中(还未读完2048), 有数据到达
            * A 读取完2048, 但还未再次试探(是否数据为空:EAGAIN)
        * 此时有新的数据到达, 不管A有多少种状态, ET将被再次激活，从而B线程被唤醒。
        * 数据将被分割,分别落到A和B
    * 场景2(Accept, 比较难以模拟,但方式和场景1类似):
        * 2个线程A and B, 同时等待事件(epoll_wait)
        * 当有新连接到达, 假设A被唤醒(ET仅通知一次)
            * 此时A从就绪队列中取走最后一个连接(应用层不可知, 需再次询问,直到返回EAGAIN), 此时listenfd事件由可读变为不可读, ET处于待通知状态
            * 当A在处理连接时，有一个新连接到达, epoll将唤醒线程B(A与B 速度竞争).
            * 若A处理足够快, B将无事可做(EAGAIN).
    * 测试方式
        run server: output/bin/example_no_support_et 0.0.0.0 8080 2
        run client: output/bin/example_no_support_client 0.0.0.0 8080 2
            server 见LT测试方式
            client 见LT测试方式
```
### 参考结果
![image](/Picture/no_support_exclusive_et.png)
```
1、场景1 如上图,数据无序的落到A和B内.
    * A、B线程全部被唤
    * A、B交互读取数据
2、场景2 比较难模拟,暂时先放过.
    * 模拟方式: 在同一个进程内, 有server也有client,通过同步或者原子或者全局变量(Volatile)
```
### [参考example](example/no_support)

## Support EPOLLEXCLUSIVE
### LT is Bad
```
LT
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


### ET is Bad
