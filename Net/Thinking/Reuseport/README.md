# SO_REUSEPORT
## 介绍
```
1、SO_REUSEPORT是端口复用技术，允许同一主机上的多个套接字绑定到相同的端口，旨在提高在多核系统上运行的多线程网络服务器应用程序的性能。
2、由内核根据连接4元组决定该分发给哪个进程。（内核3.9之后采用负载均衡策略）
```
## 能力&要求
```
1、分发策略
    * 负载均衡（默认）
    * 可控制的分发策略(用户)请关注：EBPF和CBPF
2、支持TCP/UDP、IPV4/IPV6
3、防止端口被劫持
```

## 缺陷？
* **探讨文章** - *变动accept导致连接丢失* - [goto](https://lwn.net/Articles/542738)
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
## 关于SO_REUSEPORT几篇文章
* [探讨REUSEPORT问题](https://lwn.net/Articles/542629)
* [阐述close其中一个监听导致的问题](https://engineeringblog.yelp.com/2015/04/true-zero-downtime-haproxy-reloads.html)

## 测试
```
系统版本：Linux version 4.13.0-38-generic (gcc version 7.2.0)
在TCP-SO_REUSEPORT下
1、新增accept并不会导致连接丢失。
2、在关闭listen fd 会导致丢失最大连接数:backlog size + 1

结论：
1、新增accept没有丢失连接，我认为是系统版本导致的
2、关闭listen fd 丢失backlong + 1个, 因为全联接队列大小是：backlog + 1个
后续可用linux 3.9内核测试。
```

## 避免
```
1、应避免主动关闭listen fd
2、保证在初始化所有listen fd后在提供服务
3、避免动态新增listen fd
4、通过控制分发(使用EBPF和CBPF)？待验证的方案
    * socket 提供SO_ATTACH_REUSEPORT_CBPF和SO_ATTACH_REUSEPORT_EBPF
    * https://github.com/torvalds/linux/blob/master/tools/testing/selftests/net/reuseport_bpf_cpu.c
```

# 测试用例
```
1、add_server模拟动态薪新增服务。
2、del_server模拟动态删除服务。
3、client 模拟大规模client。
```

## BPF demo - 后续补充如何通过控制bpf达成socket分发
* [DEMO](https://github.com/torvalds/linux/tree/master/tools/testing/selftests/net)
```
有兴趣的同学可以去研究
reuseaddr_conflict.c
reuseport_bpf.c
reuseport_bpf_cpu.c
reuseport_bpf_numa.c
reuseport_dualstack.c
```

## 关于BPF几篇文章介绍
* [eBPF 简史](https://blog.cloudflare.com/perfect-locality-and-three-epic-systemtap-scripts/)

## SO_REUSEPORT能解决什么问题？
```
1、
```

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