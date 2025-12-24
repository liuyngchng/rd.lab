# 1. 基本步骤

  https://pytorch.org/tutorials/beginner/basics/buildmodel_tutorial.html

**（1）准备数据样本。 **将数据样本分为训练样本和测试样本。训练样本用于模型训练，测试样本用于测试模型的泛化能力。通常还会划分一部分作为验证集（Validation Set）用于模型选择和调参。

**（2）选择人工神经网络（ANN）。**根据任务类型选择适当的神经网络架构，如全连接网络（`MLP`）、卷积神经网络（`CNN`）用于图像处理、循环神经网络（`RNN`）用于序列数据处理、`Transformer` 用于自然语言处理等。

**（3）数据训练。**包括以下核心步骤：

- 前向传播（Forward Propagation）：数据通过神经网络计算得到预测值；
- 损失计算（Loss Calculation）：使用损失函数（如均方误差MSE、交叉熵Cross-Entropy）计算预测值与真实值的差异；
- 反向传播（Backpropagation）：计算损失对模型参数的梯度；
- 参数优化（Parameter Optimization）：使用优化器（如SGD、Adam）更新网络参数。

**（4）使用测试样本集进行模型验证。**评估模型在未见过的数据上的表现，常用评估指标包括：

- 分类任务：准确率、精确率、召回率、`F1`分数、`ROC-AUC`；
- 回归任务：均方误差（`MSE`）、均方根误差（`RMSE`）、平均绝对误差（`MAE`）；
- 生成任务：结构相似性指数（`SSIM`）、峰值信噪比（`PSNR`）;

**（5）模型部署与推理。**将训练好的模型应用到实际场景中，进行实时或批量预测。

# 2. 基本概念

## 2.1 张量 Tensor

**（1）标量 (Scalar)**：零维的 Tensor，表示单个数值，例如 3。`int a = 3`, a就是个标量，标量只有大小没有方向。在`PyTorch` 中，标量是一个没有维度的张量。

```python
import torch
scalar = torch.tensor(3)  # 标量
```

**（2）向量 (Vector)**：一维的 Tensor，表示一组有序的数值。例如 `int[] a = [1, 2, 3]`， a就是个向量。可以认为a代表3维空间的一个点。

```python
vector = torch.tensor([1, 2, 3])  # 向量，形状为 (3,)
```

**（3）矩阵 (Matrix)**：二维的 Tensor，表示一个二维的数据结构（如二维图像）。例如 `int[2][2] a= [[1,2], [3,4]]`, a就是个矩阵。可以认为a代表2维空间中的2个点。

```python
matrix = torch.tensor([[1, 2], [3, 4]])  # 矩阵，形状为 (2, 2)
```

**（4）张量 (Tensor)**：三维及以上的 Tensor，可以表示更高维度的数据结构。例如 `int[2][2][2] a= [[[1,2], [3,4]],[[1,2], [3,4]]]`, a就是个矩阵。可以理解a 为更高维度空间的几何体。

```python
tensor_3d = torch.tensor([[[1, 2], [3, 4]], [[5, 6], [7, 8]]])  # 三维张量，形状为 (2, 2, 2)
```

**5）张量的重要属性**

```python
tensor = torch.randn(3, 4, 5)
print(f"Shape: {tensor.shape}")  		# 形状 (3, 4, 5)
print(f"Data type: {tensor.dtype}")  	# 数据类型
print(f"Device: {tensor.device}")  		# 存储设备 (CPU/GPU)
print(f"Size: {tensor.size()}")  		# 尺寸，同shape
print(f"Number of dimensions: {tensor.dim()}")  # 维度数
```

**（6）张量操作**

```python
# 创建张量
x = torch.zeros(2, 3)  # 全零矩阵
y = torch.ones(2, 3)   # 全一矩阵
z = torch.rand(2, 3)   # 均匀分布随机数
w = torch.randn(2, 3)  # 标准正态分布随机数

# 张量变形
reshaped = x.view(3, 2)  # 改变形状
flattened = x.flatten()  # 展平为一维

# 张量索引和切片
print(x[0, 1])  # 访问特定元素
print(x[:, 1])  # 访问第二列
print(x[0:2, 0:2])  # 访问子矩阵
```

## 2.2 tensor 乘法

**（1）矩阵乘法（Matrix Multiplication）**。 A[m, n] * B[n, p] = C[m, p]。线性代数里的矩阵乘法，必须满足A 的列数等于B的行数。

```python
# This computes the matrix multiplication between two tensors. y1, y2, y3 will have the same value
y1 = tensor @ tensor.T
y2 = tensor.matmul(tensor.T)

y3 = torch.rand_like(tensor)
torch.matmul(tensor, tensor.T, out=y3)
```

**（2）逐元素乘法（Element-wise Product）**，也叫 Hadamard product。 A[m, n] * B[m, n] = C[m, n]。, A和B里的对应位置元素逐个相乘，要求两个张量形状相同。

```python
# This computes the element-wise product. z1, z2, z3 will have the same value
z1 = tensor * tensor
z2 = tensor.mul(tensor)

z3 = torch.rand_like(tensor)
torch.mul(tensor, tensor, out=z3)
```

**（3）求和与聚合操作**

```python
tensor = torch.ones(4, 4)
print(tensor)
agg = tensor.sum()
agg_item = agg.item()
print(agg_item, type(agg_item))

# 按维度求和
sum_along_dim0 = tensor.sum(dim=0)  # 对第0维求和
sum_along_dim1 = tensor.sum(dim=1)  # 对第1维求和

# 保持维度
sum_keep_dim = tensor.sum(dim=0, keepdim=True)

# 其他聚合操作
mean_val = tensor.mean()
max_val = tensor.max()
min_val = tensor.min()
prod_val = tensor.prod()  # 所有元素的乘积
```

输出

```python
tensor([[1., 1., 1., 1.],
        [1., 1., 1., 1.],
        [1., 1., 1., 1.],
        [1., 1., 1., 1.]])
16.0 <class 'float'>
```

**（4）广播机制（Broadcasting）**当进行逐元素操作时，`PyTorch`会自动扩展形状不同的张量，使它们具有兼容的形状。

```python
# 向量 + 标量
vector = torch.tensor([1, 2, 3])
result = vector + 1  # 广播标量1到向量的每个元素

# 矩阵 + 向量
matrix = torch.tensor([[1, 2, 3], [4, 5, 6]])
row_vector = torch.tensor([10, 20, 30])
result = matrix + row_vector  # 向量被广播到矩阵的每一行
```

**（5）点积和范数**

```python
# 点积（内积）
a = torch.tensor([1, 2, 3])
b = torch.tensor([4, 5, 6])
dot_product = torch.dot(a, b)  # 1*4 + 2*5 + 3*6 = 32

# 范数
norm = torch.norm(a)  # 欧几里得范数 √(1² + 2² + 3²)
norm_l1 = torch.norm(a, p=1)  # L1范数 |1| + |2| + |3|
```

**（6）张量拼接和分割**

```python
# 拼接
a = torch.tensor([[1, 2], [3, 4]])
b = torch.tensor([[5, 6], [7, 8]])
concat_dim0 = torch.cat([a, b], dim=0)  # 沿第0维拼接
concat_dim1 = torch.cat([a, b], dim=1)  # 沿第1维拼接

# 分割
chunks = torch.chunk(tensor, chunks=2, dim=0)  # 沿维度0分成2块
split = torch.split(tensor, split_size_or_sections=2, dim=1)  # 沿维度1分割
```

**（7）梯度计算**。 `PyTorch` 张量的一个重要特性是自动求导。

```python
x = torch.tensor([1.0, 2.0], requires_grad=True)
y = x ** 2 + 3 * x
z = y.sum()
z.backward()  # 反向传播
print(x.grad)  # 梯度：dy/dx = 2x + 3
```

这些基本概念和操作是理解和使用 `PyTorch` 进行机器学习的基础，掌握它们对于后续构建和训练神经网络模型至关重要。
