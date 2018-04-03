# Net
```
Linux网络模型设计和思考该如何规避问题？
I/O多路复用主要探讨epoll。
在linux环境下，单独讨论socket、epoll是毫无意义的。
下面所有的论题都是结合socket和epoll。
```
# Getting Started
```
阐述2个问题
1、socket + epoll 值得思考的几个问题？
2、reactor 如何设计？ (设计方案很多，哪些值得我们关注的？)
```
# epoll 值得思考的几个问题(这是技术点)
* **SO_REUSEPORT** - *允许多个套接字绑定到同一个端口？（不一定是安全）* - [goto](Thinking/Reuseport)
* **EPOLLEXCLUSIVE** - *解决epoll的惊群问题？（没那么简单）* - [goto](Thinking/Exclusive)
* **epoll_ctl** - *是线程安全？*  - [goto](Thinking/epoll_ctl)


# reactor 该如何设计？

## create 2018-04-01 愚人节
## revise 2018-04-03