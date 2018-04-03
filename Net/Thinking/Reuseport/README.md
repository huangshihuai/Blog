# 缺陷
```
1、关闭其中一个listen fd，将丢失待续队列中已就绪的fd
原因：每个套接字都有自己的队列，并且当三次握手完成时，连接只被分配给其中一个队列。
我们试想：在未支持SO_REUSEEPORT时close和listen queue是否也有相同问题？
```
# 解决方案
```
1、避免程序主动关闭listen fd(非法关闭)，除非是人为干预
2、或许可以采用SO_ATTACH_REUSEPORT_CBPF和SO_ATTACH_REUSEPORT_EBPF来避免连接丢失(PS:后续将补充)
```
# 测试结果-见example
```
前提：listen queue 10，有三个进程同时listen 8080
运行后将有2个process会接受1个connect后close。(以A、B表示进程)
剩下1个继续accept后续的listen。（以C表示进程）
client 发起 100 连接。
A 和 B 分别 accept 1 + listen queue 10
C 仅仅 accept 77个
总和 A+B+C=11 + 11 + 76 = 98？（2个退出的进程？？？？）
在后续的实验中发现(修改进程数和listen queue 大小) 缺少的数量刚好和进程数相关-后续在查找资料-_-
```
# 遗留的问题
```
1、采用SO_ATTACH_REUSEPORT_CBPF和SO_ATTACH_REUSEPORT_EBPF 解决问题？
2、丢失fd不匹配
```


# 推荐文章
* [SO_REUSEPORT1](https://lwn.net/Articles/542629)
* [SO_REUSEPORT2](https://lwn.net/Articles/542738)
* [阐述close其中一个监听导致的问题](https://engineeringblog.yelp.com/2015/04/true-zero-downtime-haproxy-reloads.html)

# 谈谈想法 - 设计思路
```
1、普遍的网络模型设计（reactor）是由一个独立的acceptor（reactor）创建连接并通过dispatch分发给多个子reactor,涉及eventfd、pip等方式异步通知
2、如果采用SO_REUSEPORT需要考虑到兼容问题
```
# multi reactor + thread poll
![image](/Picture/multi_reactor_thread_pool.png)