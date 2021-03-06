# 数据库事物

|*并发异常*|*场景*|*操作*|
| :----: | :----: | :----: |
| 脏写 | 并发事物中, 事物A将事物B的提交回滚 | update |
| 脏读 | 并发事物中, 事物A对数据修改, 事物B读取事物A还未提交的数据, 但事物A回滚 | update |
| 丢失更新 | 并发事物中, 事物B 覆盖事物A的提交, 事物A丢失更新 | update |
| 不可重复读 | 并发事物中, 事物A在更改数据, 事物B在A修改前后读取数据, 其前后不一致 | update |
| 幻读 | 并发事物, 事物A在写数据, 事物B在范围查询(包含事物A写入内容), 事物B读取前后不一致 | insert |


## 脏读和不可重复读区别
`
脏读取的是事务未提交的脏数据.
不可重复读读取的是事务已经提交的数据,只不过因为数据被其他事务修改过导致前后两次读取的结果不一样
`

## 幻读和不可重复读的区别
`
不可重复读是针对确定的某一行数据而言,而幻读是针对不确定的多行数据。因而幻读通常出现在带有查询条件的范围查询中
`

# 事物隔离
|*事物隔离级别*|*脏写*|*脏读*|*不可重复读*|*幻读*|*丢失更新*|*说明*|
| :----: | :----: | :----: | :----: | :----: | :----: | :----: |
|读未提交(READ UNCOMMITTED)| / | 可能 | 可能 | 可能 | 可能 | 一个事务可以读取另一个未提交事务的数据 |
|读已提交(READ COMMITTED)| / | / | 可能 | 可能 | 可能 | 一个事务要等另一个事务提交后才能读取数据 |
|可重复读(REPEATABLE READ)| / | / | / | 可能 | / | 开始读取数据（事务开启）时，不再允许修改操作 ｜
|Serializable| / | / | / | / | / |

# 事物特性
`
原子性： 事务是数据库的逻辑工作单位,事务中的操作要么全做，要么不做。
一致性： 事务执行的结果必须是使数据库从一个一致性变到另一个一致性。
隔离性： 一个事务的执行不能干扰其他事物。即一个事务内部的操作及使用的数据对其他并发事务是隔离的，并发执行的各个事务之间不能相互干扰。
持久性： 一个事务一旦提交，他对数据库中的数据的改变应该是永久性的。接下来的其它操作或者故障不应该对其执行结果有任何影响。
`