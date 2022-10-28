<div align='center'><font size='5'>分布式系统中缓存和数据库的数据一致性问题</div>

# 背景

在分布式信息系统中，为了应对短时间内大量的请求（高并发，High Concurrency）对数据库（Database）造成的压力，一般会设置缓存（Cache），缓存拦截部分应用的数据访问，从而降低数据库的压力。但系统中只要引入了缓存，就可能存在缓存和数据库中的数据不一致的可能性。

如何在保证二者数据在任何时刻读取到的值都是一致（强一致）的前提下，又能够减轻数据库的访问压力，就成了一个需要解决的问题。涉及到缓存和数据库数据操作可能的情形如表 1 所示。

<center>表 1 应用访问缓存和数据库的情况</center>

| NO   | 操作（OP）  | 操作顺序（Sequence）       | 注释（Note）                                                 |
| ---- | ----------- | -------------------------- | ------------------------------------------------------------ |
| 1    | create（C） | （1）缓存 <br/>（2）数据库 | 先在 Cache 中新增数据，再在 DB 中新增数据。<br/>若 DB 新增数据失败，存在 DB 中丢失数据的风险。 |
| 2    | create（C） | （1）数据库<br/>（2）缓存  | 先在 DB 中新增数据，再在 Cache 中新增数据。                  |
| 3    | read（R）   | （1）缓存<br/>（2）数据库  | 先读 Cache，再读 DB。<br/>Cache 可能不是最新的值。           |
| 4    | read（R）   | （1）数据库<br/>（2）缓存  | 先读 DB，再读 Cache。<br/>没有发挥 Cache 的作用              |
| 5    | update（U） | （1）缓存<br/>（2）数据库  | 先更新 Cache，再更新 DB。<br/>若 DB 更新失败，则会导致 DB数据不是最新的。 |
| 6    | update（U） | （1）数据库<br/>（2）缓存  | 先更新 DB，再更新 Cache。<br/>若 Cache 更新失败，则会导致 Cache 数据不是最新的。 |
| 7    | delete（D） | （1）缓存<br/>（2）数据库  | 先删除 Cache，再删除 DB                                      |
| 8    | delete（D） | （1）数据库<br/>（2）缓存  | 先删除 DB，再删除 Cache                                      |

事实上，这就是一个分布式事物的一致性问题，在CAP理论中，一个分布式系统不可能同时满足一致性（C，Consistency），可用性（A，Availability）和分区容错性（P，Partition tolerance）三个基本要求，最多只能同时满足其中两个。

（1）一致性（C）指分布式信息系统的数据更新操作成功后，所有节点在同一时间的数据完全一致。

（2）可用性（A）指分布式信息系统提供的服务一直可用且能够正常响应，但不保证返回的是最新写入的数据。

（3）分区容错性（P）指分布式信息系统在遇到部分节点或部分网络分区故障的时候，仍然能够对外提供满足一致性和可用性的服务。

# 强一致性实践

保证分布式系统数据的强一致性，即任何时候从缓存和数据库中获取到的数据都是一致的。这样对于系统使用者来说，不会对其业务系统产生任何影响，但对于分布式系统来讲，其吞吐量会收到严重影响。

## 两阶段提交（Two Phase Commit）





## 三阶段提交（Three Phase Commit）



# 弱一致性实践

保证分布式系统数据的弱一致性，即通过保证数据的最终一致性，来提升系统的吞吐量，但可能会对系统使用者的业务造成影响，需要通过补偿措施降低这种影响。



# 参考文献

[1] https://www.jianshu.com/p/1a6abe6eed74;

[2] https://zhuanlan.zhihu.com/p/335617791

[3] https://xujiajia.blog.csdn.net/article/details/89601880?spm=1001.2101.3001.6661.1&utm_medium=distribute.pc_relevant_t0.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-1-89601880-blog-105242047.pc_relevant_3mothn_strategy_recovery&depth_1-utm_source=distribute.pc_relevant_t0.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-1-89601880-blog-105242047.pc_relevant_3mothn_strategy_recovery&utm_relevant_index=1