# SO_REUSEPORT
## 介绍
```
1、 SO_REUSEPORT是端口复用技术
    允许同一主机上的多个套接字绑定到相同的端口，
    旨在提高在多核系统上运行的多线程网络服务器应用程序的性能。
2、 由内核根据连接4元组决定该分发给哪个进程。（内核3.9之后采用负载均衡策略）
```
## 能力&要求
```
1、分发策略
    * 负载均衡（默认）
    * 可控制的分发策略(用户)请关注：EBPF和CBPF
2、支持TCP/UDP、IPV4/IPV6
3、防止端口被劫持
```

## 解决现有模型存在的问题？
```
单线程listen/accept，多个运行线程。
    1、性能瓶颈
    2、未能充分利用cpu cache（异步通知工作线程导致缓存丢失）
多线程共享listen和accept（例如：nginx）
    1、锁竞争
    2、负载均衡不成比例
    3、缓存跳跃
```

## 缺陷？
* **探讨文章** - *变动accept数量导致连接失效* - [goto](https://lwn.net/Articles/542738)
```
主要篇文:
The other noteworthy point is that there is a defect in the current implementation of TCP SO_REUSEPORT. 
If the number of listening sockets bound to a port changes because new servers are started or existing servers terminate, 
it is possible that incoming connections can be dropped during the three-way handshake. 
The problem is that connection requests are tied to a specific listening socket when the 
initial SYN packet is received during the handshake. 
If the number of servers bound to the port changes, then the SO_REUSEPORT logic might not route the final ACK of the handshake to the correct listening socket. In this case, the client connection will be reset, and the server is left with an orphaned request structure. A solution to the problem is still being worked on, and may consist of implementing a connection request table that can be shared among multiple listening sockets.
意思是：监听套接字的增删，可能在三次握手期间丢弃连接。(半连接队列)
```

## 为什么有缺陷？
```
1、拥有多组accept，每组都拥有各自的listen queue(半连接队列 + 全连接队列)
2、SO_REUSEPORT默认采用连接4元组确定分发队列。
场景描述：
    * 拥有3组accept队列:A、B、C
    * client发起连接(处于三次握手期间)
    * 当第一个分节符到达时(SYN),kernel通过连接4元组确定接收队列,假设为A
    * 此时,服务accept发生变动(新增和删除),假设新增D(此时存在:A、B、C、D)
    * 当第二个分节符到达时(ASK),kernel可能判定这个连接落入其他连接队列,假设为B
    * 这时 分节符1(SYN)在queue-A, 分节符2(ASK)在queue-B,导致这个连接失效
    * 此时此刻:queue-(A、、B、C)中, 还处于三次握手期间的连接, 都会失效.
```

## 解决方案
```
1、最暴力方案：不动态变动accept
2、通过控制分发，将落到CPU0上的连接导向CPU0上的accept。
    * socket 提供SO_ATTACH_REUSEPORT_CBPF和SO_ATTACH_REUSEPORT_EBPF
    * https://github.com/torvalds/linux/blob/master/tools/testing/selftests/net/reuseport_bpf_cpu.c
    * https://github.com/torvalds/linux/blob/master/tools/testing/selftests/net/reuseport_bpf_numa.c
    * 
```
# REUSEPORT可能存在的问题？
```
1、繁忙线程导致连接不及时
```

## BPF demo - 后续补充如何使用bpf控制socket分发
* [DEMO](https://github.com/torvalds/linux/tree/master/tools/testing/selftests/net)

## 关于SO_REUSEPORT几篇文章
* [探讨REUSEPORT问题](https://lwn.net/Articles/542629)
* [阐述close其中一个监听导致的问题](https://engineeringblog.yelp.com/2015/04/true-zero-downtime-haproxy-reloads.html)
* [eBPF 简史](https://blog.cloudflare.com/perfect-locality-and-three-epic-systemtap-scripts/)

## 谈谈想法 - 设计思路
```
1、普遍的网络模型设计（reactor）
    * 是由一个独立的acceptor（reactor [可以是eventloop]）等待连接
    * 通过dispatch分发给多个子reactor(eventloop)
    * dispatch一般采用异步唤醒：eventfd、pip等方式
2、采用SO_REUSEPORT？
    * 将替换掉独立的accepter和disptach分发
```
## 参见 reactor 设计
![image](/Picture/multi_reactor_thread_pool.png)