# 字节序
    存储多字节数据类型的编排方式, 它包含大端字节序和小端字节序.

## 大端字节序 - Big Endian
```
高字节存储在低地址位置, 它适合快速判断正负数
```

## 小端字节序 - Little Endian
```
低字节存储在低地址位置，它适合快速类型转换, 例如long to int
```
## 不可替换性
    大小端字节序存在各自的优势.

![image](/Picture/endian.png)
![image](/Picture/endian-exampled.png)

## 传输字节序 - (没有这个概念)
    传输字节序本意指: 网络字节序和主机字节序
    因为字节序没有被标准化(大小端字节序), 在异构机器传输, 需要将其打平字节序.

## 网络字节序
    网络字节序是指多字节数据在网络传输的编码方式.
    网络字节序默认是大端字节序.
## 主机字节序
    主机字节序-本机字节序
    它可能是大端也可能是小端.
## 传输方式
    将数据发送到远端, 需要将主机字节序转换为网络字节序(hton)
    接收远端数据时, 需将网络字节序转换为主机字节序(ntoh)
## 字符串需要字节序转换？
    它是不需要, 字节序只影响多字节数据类型, 不影响单字节字符串(char).
    宽字节字符串跟其编码有关系, 也不需要字节转换.