# RPS & RFS
```
相关知识 网卡绑定cpu
```

# 网络内核优化参数
```
可查看: /proc/sys/net
TCP缓冲估算: https://www.cnblogs.com/whuqin/p/5580895.html
```

# 内核优化说明 
## net.core.netdev_max_backlog = 30000
每个网络接口接收数据包的速率比内核处理这些包的速率快时, 允许送到队列的数据包的最大数目

## net.core.somaxconn = 262144
用来限制监听(LISTEN)队列最大数据包的数量, 超过这个数量就会导致链接超时或者触发重传机制, 此处的优先级高于用户态的listen

## net.core.rmem_default = 8388608
接收套接字缓冲区大小的默认值(以字节为单位)

## net.core.wmem_default = 8388608
发送套接字缓冲区大小的默认值(以字节为单位)

## net.core.rmem_max=16777216
接收套接字缓冲区大小的最大值(以字节为单位)

## net.core.wmem_max=16777216
发送套接字缓冲区大小的最大值(以字节为单位)

## net.ipv4.ip_local_port_range = 1024 65536
用于向外连接的端口范围

## net.ipv4.tcp_mem = 786432 1048576 1572864
确定 TCP 栈应该如何反映内存使用, 此处分别为3G、4G、6G; 每个值的单位都是内存页(通常是 4KB).
第一个值是内存使用的下限.
第二个值是内存压力模式开始对缓冲区使用应用压力的上限.
第三个值是内存上限.在这个层次上可以将报文丢弃, 从而减少对内存的使用.

## net.ipv4.tcp_rmem=4096 87380 16777216
TCP接收缓冲区, 3个字段分别是min, default, max.
Min: 为TCP socket预留用于接收缓冲的内存最小值, 即使在内存出现紧张情况下TCP socket都至少会有这么多数量的内存用于接收缓冲.
Default: 为TCP socket预留用于接收缓冲的内存数量, 一般要低于net.core.rmem_default的值.
Max: 为TCP socket预留用于接收缓冲的内存最大值.一般要低于net.core.rmem_max.

## net.ipv4.tcp_wmem=4096 65536 16777216
TCP发送缓冲区, 3个字段分别是min, default, max.
Min: 为TCP socket预留用于发送缓冲的内存最小值.即使在内存出现紧张情况下TCP socket都至少会有这么多数量的内存用于接收缓冲.
Default: 为TCP socket预留用于发送缓冲的内存数量, 一般要低于net.core.wmem_default的值.
Max: 为TCP socket预留用于发送缓冲的内存最大值.一般要低于net.core.wmem_max.

## net.ipv4.tcp_fin_timeout = 10
如果套接字由本端要求关闭, 这个参数决定了它保持在FIN-WAIT-2状态的时间. 对端可以出错并永远不关闭连接, 甚至意外宕机. 缺省值是60秒

## net.ipv4.tcp_tw_recycle = 1
开启TCP连接中TIME-WAIT sockets的快速回收, 默认为0, 表示关闭

## net.ipv4.tcp_tw_reuse = 1
表示是否允许重新应用处于TIME-WAIT状态的socket用于新的TCP连接.

## net.ipv4.tcp_max_tw_buckets = 6000
系统在同时所处理的最大timewait sockets 数目.如果超过此数的话, time-wait socket 会被立即砍除并且显示警告信息.之所以要设定这个限制, 纯粹为了抵御那些简单的 DoS 攻击, 千万不要人为的降低这个限制, 不过, 如果网络条件需要比默认值更多, 则可以提高它(或许还要增加内存)

## net.ipv4.tcp_timestamps = 0
时间戳可以避免序列号的卷绕. 一个1Gbps 的链路肯定会遇到以前用过的序列号.时间戳能够让内核接受这种“异常”的数据包.这里需要将其关掉.

## net.ipv4.tcp_window_scaling = 1
支持更大的TCP窗口. 如果TCP窗口最大超过65535(64KB), 必须设置该数值为1.

## net.ipv4.tcp_sack = 1
表示是否启用有选择的应答)Selective Acknowledgment), 这可以通过有选择地应答乱序接收到的报文来提高性能)这样可以让发送者只发送丢失的报文段); )对于广域网通信来说)这个选项应该启用, 但是这会增加对 CPU 的占用

## net.ipv4.tcp_no_metrics_save=1
默认情况下一个tcp连接关闭后,把这个连接曾经有的参数比如慢启动门限snd_sthresh,拥塞窗口snd_cwnd 还有srtt等信息保存到dst_entry中, 只要dst_entry 没有失效,下次新建立相同连接的时候就可以使用保存的参数来初始化这个连接.通常情况下是关闭的.

## net.ipv4.tcp_keepalive_time = 1200
当keepalive起用的时候, TCP发送keepalive消息的频度.缺省是2小时, 改为20分钟

## net.ipv4.tcp_syncookies = 1
当出现SYN等待队列溢出时, 启用cookies来处理, 可防范少量SYN攻击, 默认为0, 表示关闭(非性能问题)

## net.ipv4.tcp_max_orphans = 262144
系统中最多有多少个TCP 套接字不被关联到任何一个用户文件句柄上.如果超过这个数字, 孤儿连接将即刻被复位并打印出警告信息.这个限制仅仅是为了防止简单的DoS 攻击, 不能过分依靠它或者人为地减小这个值, 更应该增加这个值(如果增加了内存之后).

## net.ipv4.tcp_max_syn_backlog = 262144
表示那些尚未收到客户端确认信息的连接(SYN消息)队列的长度, 默认为1024, 加大队列长度为262144, 可以容纳更多等待连接的网络连接数.

## net.ipv4.tcp_synack_retries = 2
为了打开对端的连接, 内核需要发送一个SYN 并附带一个回应前面一个SYN 的ACK.也就是所谓三次握手中的第二次握手.这个设置决定了内核放弃连接之前发送SYN+ACK 包的数量.

## net.ipv4.tcp_syn_retries = 2
在内核放弃建立连接之前发送SYN 包的数量

##  web服务器收到了大量的连接, 在启用了iptables的情况下, iptables会把所有的连接都做链接跟踪处理, 这样iptables就会有一个链接跟踪表
```
net.ipv4.netfilter.ip_conntrack_tcp_timeout_established=1800
链接跟踪表保存时间, 单位: 秒
net.ipv4.netfilter.ip_conntrack_max=131070
链接跟踪表最大数目
```


## other
```
kernel.sysrq = 0
允许按住Alt + PrtSc键打印内核信息, 除非是调试, 一般要关闭此功能
kernel.core_uses_pid = 1
控制core文件的文件名中是否添加pid作为扩展.文件内容为1, 表示添加pid作为扩展名, 生成的core文件格式为core.xxxx; 为0则表示生成的core文件同一命名为core
kernel.msgmnb = 65536
每个消息队列的最大字节限制.
kernel.msgmax = 65536
从一个进程发送到另一个进程的消息的最大长度)bytes).进程间的消息传递是在内核的内存中进行的, 不会交换到磁盘上, 所以如果增加该值, 则将增加操作系统所使用的内存数量.
kernel.shmmax = 68719476736
表示内核所允许的最大共享内存段的大小)bytes)
kernel.shmall = 4294967296
系统上可以使用的共享内存的总量)bytes)
kernel.randomize_va_space = 1
将该值设置为 0 可禁用地址空间布局随机化.DB2 数据服务器依赖特定共享内存对象的固定地址, 地址空间布局随机化会导致有些活动发生错误
net.ipv4.ip_forward = 0
0表示禁用 IPv4 包转送
net.ipv4.neigh.default.gc_stale_time=120
ARP参数, 检查一次相邻层记录的有效性的周期.当相邻层记录失效时, 将在给它发送数据前, 再解析一次.缺省值是60秒.

net.ipv4.conf.all.rp_filter=0  
net.ipv4.conf.all.arp_announce=2
net.ipv4.conf.lo.arp_announce=2

ref: https://blog.csdn.net/lcx46/article/details/12418873
```

# 内核优化 - [/etc/sysctl.conf]
```
net.ipv4.tcp_max_syn_backlog = 120000
net.core.netdev_max_backlog = 120000
net.core.somaxconn = 12000
net.ipv4.tcp_fin_timeout = 2
net.ipv4.tcp_tw_reuse = 1
net.ipv4.tcp_tw_recycle = 1
net.ipv4.tcp_max_tw_buckets = 1000
net.ipv4.tcp_abort_on_overflow = 1
net.ipv4.tcp_sack = 1
net.ipv4.tcp_rmem = 4096 8192 32768
net.ipv4.tcp_wmem = 4096 8192 32768
net.core.optmem_max = 8192
net.ipv4.tcp_timestamps = 0
net.ipv4.tcp_mem = 3075840  4101120  12303360
net.ipv4.tcp_keepalive_time = 30
net.ipv4.tcp_keepalive_probes = 5
net.ipv4.tcp_keepalive_intvl = 15
net.netfilter.nf_conntrack_tcp_timeout_last_ack = 3
net.ipv4.tcp_synack_retries = 1
net.ipv4.tcp_syn_retries = 1
net.ipv4.ip_local_port_range = 1024     65535
net.nf_conntrack_max = 6553600
net.netfilter.nf_conntrack_max = 6553500
net.netfilter.nf_conntrack_tcp_timeout_established = 180
```
# 内核优化 - [/etc/sysctl.conf][option]
```
net.core.rmem_default = 8388608
net.core.wmem_default = 8388608
net.core.rmem_max=16777216
net.core.wmem_max=16777216
net.ipv4.tcp_no_metrics_save=1
net.ipv4.tcp_syncookies = 1
net.ipv4.tcp_max_orphans = 262144
net.ipv4.netfilter.ip_conntrack_tcp_timeout_established=1800
net.ipv4.netfilter.ip_conntrack_max=131070
kernel.sysrq = 0
kernel.core_uses_pid = 1
kernel.msgmnb = 65536
kernel.msgmax = 65536
kernel.shmmax = 68719476736
kernel.shmall = 4294967296
kernel.randomize_va_space = 1
kernel.exec-shield = 1
net.ipv4.ip_forward = 0
net.ipv4.neigh.default.gc_stale_time=120
net.ipv4.conf.default.accept_source_route = 0
net.ipv4.conf.default.rp_filter=0
net.ipv4.conf.all.rp_filter=0  
net.ipv4.conf.all.arp_announce=2
net.ipv4.conf.lo.arp_announce=2
```

# 内核优化 - [/etc/security/limits.conf]
    soft    nofile          1200000
    hard    nofile          1200000

# 协议优化
    关闭TCP快速回收
    Init RTO不低于3秒
    初始拥塞控制窗口不小于10
    Socket buffer > 64k
    TCP滑动窗口可变
    控制发包大小在1400字节以下, 避免分片
 # 协议优化的原则总结下来是这么几条: 
    连接重用
    并发连接控制
    超时控制
    包头精简
    内容压缩

# 参考
    https://segmentfault.com/a/1190000006733978