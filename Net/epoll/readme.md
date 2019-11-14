# epoll
```
    epoll 对新手来说是一个麻烦事, 半知不解中夹杂着疑问.
    我将介绍以下部分:
        * epoll 的线程安全
        * epoll 的一些flag(ET LT EPOLLONESHOT)
        * epoll 是否解决了鲸群
```

## epoll线程安全吗?
```
    首先它可以是线程安全也可以是线程非安全, 因为新版本的epoll解决了线程安全问题(从3.5内核上看), 但它有线程非安全的版本.
        * 在看许多开源网络框架, 会发现都是使用线程非安全的设计方式. - 所有epoll操作都在I/O线程中执行
```
## [epoll patch](https://lore.kernel.org/patchwork/patch/351273/)
```
简单的说是: 会丢失感兴趣的文件描述福 - (原因: 内存屏障)
```

## epoll 的一些flag(ET LT EPOLLONESHOT)
### ET

### LT

### EPOLLONESHOT

## epoll 是否解决了鲸群