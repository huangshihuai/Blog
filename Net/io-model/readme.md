# I/O model
```
I/O model 概念上很多: 同步 I/O & 异步 I/O, 阻塞I/O & 非阻塞I/O, I/O 操作
同步I/O 和 异步I/O 指的是I/O操作
阻塞I/O 和 非阻塞I/O 指的是I/O状态
```

## 同步/异步 & 阻塞/非阻塞
```
为什么要聊这个?
    * 我遇到过有些同学并不能把它讲明白. 例如说 阻塞是同步？ 异步是非阻塞? 反之也是?(这里不能把他们混为谈论)
那么它们是什么?
    * 同步/异步: 它们是描述一个任务的过程. 在完成这个任务中, 它可以是同步也可以是异步.
    * 阻塞/非阻塞: 它们是描述一种执行状态.
Synchronous && Asynchronous.
    Process of performing one thing.
Blocking && Non-Blocking.
    It’s base state to performing one thing
```

## 同步阻塞I/O
![image](/Picture/blockingIO.png)
```
    同步是指在数据拷贝上是同步操作(copy data from kernel to user)
    阻塞是指在等待I/O数据是阻塞状态(从调度角度看, IO阻塞, 进程被挂起).
```

## 同步非阻塞I/O
![image](/Picture/nonBlockingIO.png)
```
    同步是指在数据拷贝上是同步操作(copy data from kernel to user)
    非阻塞是指在等待I/O数据是非阻塞状态.
```

## Asynchronous I/O
```
An asynchronous I/O operation does not cause the request process to be blocked.
在I/O操作上不能被阻塞. I/O数据在用户态.
```
## Synchronous I/O
```
A synchronous I/O operation causes the requesting process to be blocked until that I/O operation completes.
在I/O操作上将被阻塞, 直到I/O操作完成. I/O数据从内核态拷贝到用户态
```

## I/O 模型
![image](/Picture/io-all.png)