# 1. setup

官方网站详见  https://milvus.io/docs。

docker 安装文档详见  https://milvus.io/docs/install_standalone-docker.md。

```sh
# pull images
docker pull milvusdb/milvus:v2.6.0
# Download the installation script
curl -sfL https://raw.githubusercontent.com/milvus-io/milvus/master/scripts/standalone_embed.sh -o standalone_embed.sh
# Start the Docker container
./standalone_embed.sh start
# check
docker ps | grep milvus
dced5a07d916   milvusdb/milvus:v2.6.0   "/tini -- milvus run…"   2 minutes ago   Up 2 minutes (healthy)   0.0.0.0:2379->2379/tcp, :::2379->2379/tcp, 0.0.0.0:9091->9091/tcp, :::9091->9091/tcp, 0.0.0.0:19530->19530/tcp, :::19530->19530/tcp   milvus-standalone
```

milvus server **19530**

etcd **2379**

webui **9091**

查看服务状态， http://127.0.0.1:9091/webui/

# 2. package

## 2.1 db packge

```sh
# 推荐
pip install -U pymilvus
# 不推荐
pip install -U pymilvus-orm
```

这个包主要包含python client 库 和 Milvus Lite库。

`pymilvus` 和 `pymilvus-orm` 的关系是这样的，`pymilvus` (2.x+) 是当前（2023年起）推荐的，包含完整的 Milvus 客户端功能（低级API + 高级ORM）；**`pymilvus-orm` **是旧版 ORM 组件，仅包含旧版 ORM 接口（Milvus 1.x 时代的分离包）自 Milvus 2.0 起已废弃，功能合并到主库，不再需要单独安装。

依赖的package 如下所示：

```python
pip download pymilvus
Collecting pymilvus
  Downloading pymilvus-2.6.0-py3-none-any.whl.metadata 
  ......
Successfully downloaded pymilvus grpcio milvus-lite pandas protobuf python-dotenv setuptools ujson numpy python-dateutil pytz tzdata tqdm six
```



## 2.2 其他组件

```
pip install "pymilvus[model]"
```

这里安装的是 **PyMilvus 的模型插件依赖**，主要包含：

**核心功能**

- 启用 Milvus 的**内置向量生成模型集成**（如 Hugging Face `sentence-transformers`）
- 支持直接通过 Milvus **自动将文本转为向量**（无需额外预处理）

**关键依赖**

```sh
# 主要安装的包：
sentence-transformers >=2.2.2  # 文本嵌入模型框架
transformers >=4.37.0          # Hugging Face 模型库
tokenizers >=0.15.0            # 文本分词工具
```

# 3. basic concepts

（1）**Collection**（集合）。 相当于RDBMS 的 table，是存储向量和标量数据的容器。

（2）**Dimensionality**（维度）。维度指的是向量的长度（元素数量），决定向量空间的结构。例如，`[0.1, -0.2, 0.8, ..., 0.4]` 若包含 **128** 个浮点数 → 维度为 **128**。

（3）**Index Types**（索引类型）。索引的作用是加速向量检索的算法结构，索引类型直接影响查询速度和精度。

常用类型有以下集中

- `FLAT`：精确检索（全量扫描），精度100%但速度慢。

- `HNSW（Hierarchical Navigable Small World）`：层级图结构，速度快且精度高（近似检索），但没有FLAT 的精度高。

- `IVF_FLAT（Inverted File，倒排文件索引 + 原始向量存储）`：聚类分桶检索，平衡速度与精度。

  索引类型选择，需要分场景， 

  | 场景                 | 推荐索引   | 推荐度量 |
  | -------------------- | ---------- | -------- |
  | 人脸识别（精度优先） | `HNSW`     | `COSINE` |
  | 推荐系统（速度优先） | `IVF_FLAT` | `IP`     |
  | 小数据集测试         | `FLAT`     | `L2`     |

（4）**Distance Metrics**（距离度量）。

- **定义**：计算向量间相似度的数学方法。

- 常用类型：
  - `L2`（欧氏距离）：向量各维度差的平方和 → 值越小越相似。
  
  - `IP`（内积）：向量点积 → 值越大越相似。
  
  - `COSINE`（余弦相似度）：向量方向一致性 → **1** 表示完全同向，**-1** 表示反向。
  
    
  

sdfsdfs