# 1. 基本步骤

  https://pytorch.org/tutorials/beginner/basics/buildmodel_tutorial.html

**（1）准备数据样本。**将数据样本分为训练样本和测试样本。训练样本用于模型训练， 测试样本用于测试模型的泛化能力。

（2）选择人工神经网络（ANN）。

（3）数据训练。

（4）使用测试样本集进行模型验证。

# 2. 基本概念

## 2.1 张量 Tensor

**（1）标量 (Scalar)**：零维的 Tensor，表示单个数值， 例如 3。`int a = 3`, a就是个标量，标量只有大小没有方向。

**（2）向量 (Vector)**：一维的 Tensor，表示一组有序的数值。例如 `int[] a = [1, 2, 3]`， a就是个向量。可以认为a代表3维空间的一个点。

**（3）矩阵 (Matrix)**：二维的 Tensor，表示一个二维的数据结构（如二维图像）。例如 `int[2][2] a= [[1,2], [3,4]]`, a就是个矩阵。可以认为a代表2维空间中的2个点。

**（4）张量 (Tensor)**：三维及以上的 Tensor，可以表示更高维度的数据结构。例如 `int[2][2][2] a= [[[1,2], [3,4]],[[1,2], [3,4]]]`, a就是个矩阵。可以理解a 为更高维度空间的几何体。

## 2.2 tensor 乘法

​		（1）矩阵乘法（Matrix Multiplication）。 A[m, n] * B[n, p] = C[m, p]。线性代数里的矩阵乘法，必须满足A 的列数等于B的行数。

```python
# This computes the matrix multiplication between two tensors. y1, y2, y3 will have the same value
y1 = tensor @ tensor.T
y2 = tensor.matmul(tensor.T)

y3 = torch.rand_like(tensor)
torch.matmul(tensor, tensor.T, out=y3)
```

​		（2）element-wise product，也叫 Hadamard product。A[m, n] * B[m, n] = C[m, n]。, A和B里的对应位置元素逐个相乘。

```python
# This computes the element-wise product. z1, z2, z3 will have the same value
z1 = tensor * tensor
z2 = tensor.mul(tensor)

z3 = torch.rand_like(tensor)
torch.mul(tensor, tensor, out=z3)
```

​		（3）求和

```python
tensor = torch.ones(4, 4)
print(tensor)
agg = tensor.sum()
agg_item = agg.item()
print(agg_item, type(agg_item))
```

输出

```python
tensor([[1., 1., 1., 1.],
        [1., 1., 1., 1.],
        [1., 1., 1., 1.],
        [1., 1., 1., 1.]])
16.0 <class 'float'>
```

