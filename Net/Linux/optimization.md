# RPS & RFS
    

# 内核优化 - [/etc/sysctl.conf]
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
# 内核优化 - [/etc/security/limits.conf]
    soft    nofile          1200000
    hard    nofile          1200000

# 协议优化
    关闭TCP快速回收
    Init RTO不低于3秒
    初始拥塞控制窗口不小于10
    Socket buffer > 64k
    TCP滑动窗口可变
    控制发包大小在1400字节以下，避免分片
 # 协议优化的原则总结下来是这么几条：
    连接重用
    并发连接控制
    超时控制
    包头精简
    内容压缩

# 参考
    https://segmentfault.com/a/1190000006733978