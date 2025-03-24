# 1. 基本思想

##  1.1 问题数学描述

有一组样本数据 ,假定为 $n$ 组， 如下所示：

```python
x = [1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0 , 10.0]
y = [2.0, 4.1, 6.3, 7.9, 9.8, 11.5, 14.3, 16.6, 18.1, 21]
```

即：
$$
x_1 = 1.0,  x_2=2.0,...\\
y_1 = 2.0, y_2 = 4.1,....
$$
 需要推断一个函数，能够满足这一组样本值的特征，这个推断的过程叫做**机器学习**。

从 $[x, y]$ 的散点图来看，感觉是个一元一次方程（即采用了**线性模型**），如图 1所示。

![x_1.png](./pic/x_1.png)

<center>图1 二维线性函数图像</center>

假定为
$$
y=wx+b \tag {1-1}
$$
只要能计算出w, b，那么就得到了这个函数，同时从数据分布来看，公式（1）为一条直线，对于某个给定的值$x_i$，计算出的值 $y'_i$ 跟样本集中对应的实际值 $y_i$ 是有误差的 。通常通过均方差（**最小二乘法**）来评价这种误差，即
$$
loss = \sum^{n}_{i=1}(y'_i-y_i)^2\tag {1-2}
$$
公式（2）在机器学习中称之为**损失函数**。需要求解的就是组合
$$
[w, b]\tag {1-3}
$$
使得公式（2）中的函数值 $loss$ 达到其最小值。为了便于说明问题，将公式（1）代入公式（2），即
$$
\begin{align}
loss &= \sum^{n}_{i=1}(y'_i-y_i)^2\\
&=\sum^{n}_{i=1}(wx_i+b-y_i)^2
\tag {1-4}
\end{align}
$$
公式（5）中，$n,x_i, y_i$ 都是由样本集决定的，即为已知参数。$w, b$ 为未知参数。

##  1.2 简化计算

为了简化问题方便推导，只使用样本集中的 2个样本值，即 $n = 2$，代入样本数据，计算 $loss$ 的值： 
$$
\begin{align}
loss &= (w\times1.0 + b -2.0)^2+(w\times2.0 + b -4.1)^2\\
&=(w+b-2)^2+(2w+b-4.1)^2\\
&=(推导省略...）\\
&=10w^2+4b^2+12wb-40.08w-24.4b+41.62
\end{align}\tag{1-5}
$$
问题转化为求公式（6）这个函数的最小值了，将公式（6）中的$loss$ 换成 $z$,  $w,b$分别换成 $x, y$,（只是为了方便理解而已，不换也可以），即：
$$
z=10w^2+4b^2+12wb-40.08w-24.4b+41.62\tag{1-6}
$$
求公式（7）的函数最小值。公式（7）函数达到其最小值时其自变量 $x,y$ 的值，即公式（6）中的 $w, b$ 的值，就是符合2个样本值（$n=2$）的拟合函数的待定系数。公式（7）函数的三维图像如图2所示。

![5x2_2y2](./pic/5x^2_2y^2.png)

<center>图 2 函数三维图像</center>

# 2. 梯度下降法

##  2.1 导数含义

公式（5）求解的过程，引入了**梯度下降法**。公式（5）是一个关于 $w, b$ 的二元一次方程
$$
\begin{align}
loss &=\sum^{n}_{i=1}(wx_i+b-y_i)^2
\tag {2-1}
\end{align}
$$
对于给定的样本集，$n, x_i,y_i$ 都是已知的参数，$w,b $ 是自变量， $loss$ 是关于 $w,b$ 的函数。

梯度下降法的基本思想就是沿着函数斜率下降的方向前进，直到到达斜率为 $0$ 的点，则该点就是函数的最小值点，前提是函数存在最小值。那么什么是斜率呢？斜率就是函数对自变量进行求导，例如，对于一元（元，即自变量的个数）二次（二次，即函数的最高次幂）函数 $f(x)=x^2$ ，对于函数曲线上的任何一个点 $x$ , 此处的斜率为 $f(x)$ 对 $x$ 求导，即： 
$$
\frac{df(x)}{dx} = \frac{d(x^2)}{dx} = 2x \tag {2-2}
$$
以此类推，对于二元二次函数 $f(x, y)=x^2 + y^2$, 则是求偏导。$f(x,y)$是一个三维空间的曲面，如图 3 所示。

![x^2+y^2](./pic/x^2_y^2.png)

<center>图 3 函数三维图像</center>

偏导就是在曲面上某个点的切面和坐标轴所在面的交叉线方向的斜率，例如，在 $[x,y, z]$ 点对自变量 $x$ 求偏导，就是函数的曲面上该点的切面与 $z$ 轴和 $x$ 轴形成的平面的交叉线的斜率。由于函数曲面的切面上有很多方向，偏导就是指定了某一个方向。

##  2.2 求导

公式（5）对 $w$ 求导:
$$
\begin{align}
\frac{d(loss)}{dw} &=\frac{d[\sum^{n}_{i=1}(wx_i+b-y_i)^2]}{dw}\\
&=\sum^{n}_{i=1}\frac{d[(wx_i+b-y_i)^2]}{dw}\\
&=2\sum^{n}_{i=1}(x_i+b-y_i)\\
\tag 5
\end{align} \tag {2-3}
$$

公式（5）对 $b$ 求导
$$
\begin{align}
\frac{d(loss)}{db} &=\frac{d[\sum^{n}_{i=1}(wx_i+b-y_i)^2]}{db}\\
&=\sum^{n}_{i=1}\frac{d[(wx_i+b-y_i)^2]}{db}\\
&=2\sum^{n}_{i=1}(1+wx_i-y_i)\\
\end{align} \tag {2-4}
$$

# 3. 实践



以 pytorch 官网 https://pytorch.org/tutorials/beginner/pytorch_with_examples.html 的 $sin(x)$ 函数拟合为例，进行实践

##  3.1 样本数据

```sh
x = np.linspace(-math.pi, math.pi, 2000)
y = np.sin(x)
```

x, y 为 2000组 样本数据，作为进行函数拟合的输入

##  3.2 目标函数

采用的目标函数为

```sh
y_pred = a + b * x + c * x ** 2 + d * x ** 3
```

即
$$
y=a + bx+cx^2+dx^3\tag {3-1}
$$
对于任意一个 $x_i$, 按照拟合函数计算的函数值为 $y'$, 记为 $y'$ 是为了和样本数据 $y_i$ 进行区别。
$$
y'=a+bx_i+cx_i^2+dx_i^3\tag {3-2}
$$


##  3.3 损失函数

计算方差，作为损失函数  $loss$

```sh
 loss = np.square(y_pred - y).sum()
```

即
$$
\begin{align}
loss&=\sum_{i=0}^{2000}{(y'-y_i)^2}\\
&=\sum_{i=0}^{2000}{(a+bx_i+cx_i^2+dx_i^3-y_i)^2}
\end{align} \tag {3-3}
$$
其中 $a, b, c, d$ 为未知的参数, $x_i, y_i$ 为样本数据， 当 $loss$ 达到其最小值时的 $a, b, c, d$ 即为最佳的参数，于是问题转化为求公式（3-3）的最小值。

##  3.4 求梯度

###  3.4.1 多项式求导数的一般性规律

为简化后面的计算，首先计算 $f(a,b) = (a+bx_i-y_i)^2分别对$$a, b$  求偏导，寻找一般性规律:
$$
\begin{align}
f(a,b)&=(a+bx_i-y_i)^2\\&=a^2+(bx_i-y_i)^2+2a(bx_i-y_i)\\
&=a^2+b^2x_i^2+y_i^2-2bx_iy_i+2abx_i-2ay_i\\
&=a^2+b^2x_i^2+y_i^2+2abx_i-2bx_iy_i-2ay_i\tag{3-4}
\end{align}
$$
接下来分别求$f(a,b)$ 对 $a, b$ 的偏导，对 $a$ 求偏导， $b,x_i, y_i$ 均为常数（常数导数为 0 ），则：
$$
\begin{align}
\frac{\partial{f(a,b)}}{\partial{a}}&=\frac{\partial{(a^2+b^2x_i^2+y_i^2+2abx_i-2bx_iy_i-2ay_i)}}{\partial{a}}\\
&=2a+2bx_i-2y_i\\
&=2(a+bx_i-y_i)\tag{3-5}
\end{align}
$$
对 $b$ 求偏导， $a,x_i, y_i$ 均为常数（常数导数为 0 ），则：
$$
\begin{align}
\frac{\partial{f(a,b)}}{\partial{b}}&=\frac{\partial{(a^2+b^2x_i^2+y_i^2+2abx_i-2bx_iy_i-2ay_i)}}{\partial{b}}\\
&=2bx_i^2+2ax_i-2x_iy_i\\
&=2x_i(a+bx_i-y_i)\tag{3-6}
\end{align}
$$
综合公式（3-4）、（3-5）、（3-6），可以得到：
$$
\begin{align}
\frac{\partial{[(a+bx_i-y_i)^2]}}{\partial{a}}=2(a+bx_i-y_i)\tag{3-7}
\end{align}
$$

$$
\begin{align}
\frac{\partial{[(a+bx_i-y_i)^2]}}{\partial{b}}=2x_i(a+bx_i-y_i)\tag{3-8}
\end{align}
$$

从公式（3-4）、（3-5）中寻找一般性规律（这个数据学上是可以推导出来的，感兴趣的可以一步一步推导），依次类推，可以得到：
$$
\begin{align}
\frac{\partial{[(a+bx_i+cx_i^2-y_i)^2]}}{\partial{c}}=2x_i^2(a+bx_i-y_i)\tag{3-9}
\end{align}
$$


###  3.4.2 求导公式

导数公式（可导函数和的导数等于各个函数分别求导的和），即：
$$
[f(x)+g(x)]'=f'(x)+g'(x) \tag{3-10}
$$

###  3.4.3 求梯度

```python
grad_a = grad_y_pred.sum()
```

含义为求函数 $loss$ 对 $a$ 的偏导（将 $b, c, d$ 都看作是常数），求解 $loss$ 对 $a$ 的偏导，利用公式（3-7）的一般性规律以及公式（3-10），可以得到：
$$
\begin{align}
grad\_a=\frac{\partial{loss}}{\partial{a}}
&=\frac{\partial{\sum_{i=0}^{2000}{(a+bx_i+cx_i^2+dx_i^3-y_i)^2}}}{\partial{a}}\\
&=\sum_{i=0}^{2000}\frac{\partial{(a+bx_i+cx_i^2+dx_i^3-y_i)^2}}{\partial{a}}\\
&=\sum_{i=0}^{2000}2(a+bx_i+cx_i^2+dx_i^3-y_i)\\
&=2\sum_{i=0}^{2000}a+bx_i+cx_i^2+dx_i^3-y_i\\
\end{align} \tag {3-11}
$$
将公式（3-2）代入，得到:
$$
\begin{align}
grad\_a=\frac{\partial{loss}}{\partial{a}}

&=2\sum_{i=0}^{2000}(y'-y_i)\\
\end{align} \tag {3-12}
$$

```python
grad_b = (grad_y_pred * x).sum()
```

即求函数 $loss$ 对 $b$ 的偏导（将 $a, c, d$ 都看作是常数），利用公式（3-7）、（3-8）、（3-9）的一般性规律以及公式（3-10），可以得到:
$$
\begin{align}
grad\_b=\frac{\partial{loss}}{\partial{b}}
&=\frac{\partial{\sum_{i=0}^{2000}{(a+bx_i+cx_i^2+dx_i^3-y_i)^2}}}{\partial{b}}\\
&=\sum_{i=0}^{2000}\frac{\partial{(a+bx_i+cx_i^2+dx_i^3-y_i)^2}}{\partial{b}}\\
&=\sum_{i=0}^{2000}2(a+bx_i+cx_i^2+dx_i^3-y_i)\\
&=2\sum_{i=0}^{2000}x_i(a+bx_i+cx_i^2+dx_i^3-y_i)\\
&=2\sum_{i=0}^{2000}x_i(y'-y_i)
\end{align} \tag {3-13}
$$
推而广之，

```python
grad_c = (grad_y_pred * x ** 2).sum()
grad_d = (grad_y_pred * x ** 3).sum()
```

即求函数 $loss$ 对 $c,d$ 的偏导, 对  $c$ 求偏导， 即：
$$
\begin{align}
grad\_c=\frac{\partial{loss}}{\partial{c}}
&=\frac{\partial{\sum_{i=0}^{2000}{(a+bx_i+cx_i^2+dx_i^3-y_i)^2}}}{\partial{c}}\\
&=2\sum_{i=0}^{2000}x_i^2(y'-y_i)
\end{align} \tag {3-14}
$$
对 $d$ 求偏导， 即：
$$
\begin{align}
grad\_d=\frac{\partial{loss}}{\partial{d}}
&=\frac{\partial{\sum_{i=0}^{2000}{(a+bx_i+cx_i^2+dx_i^3-y_i)^2}}}{\partial{d}}\\
&=2\sum_{i=0}^{2000}x_i^3(y'-y_i)
\end{align} \tag {3-15}
$$

##  3.5 梯度下降法

对于一个类似于图3 的凹函数（类似于一个小山谷），沿着梯度下降的方向前进（沿着山坡的斜坡向下走），总能找到一个局部或全局的最低点（山谷谷底）。

最初的 $a,b,c,d$ 的值是随意给的，见代码

```python
a = np.random.randn()
b = np.random.randn()
c = np.random.randn()
d = np.random.randn()
```

对应的物理意义是公式（3-3）中的损失函数 $loss$ 的曲面上随机地找了一个点，这个点的坐标为 $[a_0,b_0,c_0,d_0]$。

```python
learning_rate = 1e-6
```

即  $learning\_rate=10^{-6}$

```python
a -= learning_rate * grad_a
b -= learning_rate * grad_b
c -= learning_rate * grad_c
d -= learning_rate * grad_d
```

即表示步进至下一个点$[a_1,b_1, c_1, d_1]$, 其中
$$
\begin{align}
a_1&=a_0-learning\_rate * grad\_a\\
&=a_0-learning\_rate \times 2\sum_{i=0}^{2000}(y'-y_i)\\
&=a_0-10^{-6} \times 2\sum_{i=0}^{2000}(a_0+b_0x_i+c_0x_i^2+d_0x_i^3-y_i)\tag {3-16}
\end{align}
$$
公式（3-16）中的所有变量均有值，$a_1$是可以求出来的，同理：
$$
\begin{align}
b_1&=b_0-10^{-6} \times 2\sum_{i=0}^{2000}x_i(a_0+b_0x_i+c_0x_i^2+d_0x_i^3-y_i)\tag {3-17}\\
c_1&=c_0-10^{-6} \times 2\sum_{i=0}^{2000}x_i^2(a_0+b_0x_i+c_0x_i^2+d_0x_i^3-y_i)\tag {3-18}\\
d_1&=d_0-10^{-6} \times 2\sum_{i=0}^{2000}x_i^3(a_0+b_0x_i+c_0x_i^2+d_0x_i^3-y_i)\tag {3-18}\\
\end{align}
$$
计算出下一个点的坐标 $[a_1, b_1, c_1, d_1]$ 后，再计算损失函数 $loss$ 的值（计算损失函数的值称为前向传播），依次类推，直到到达损失函数 $loss$ 对各个变量的导数为0 （到达了山谷谷底）。

##  3.6 运行结果

运行程序，该程序中一共迭代了 2000 次，可以看到 loss呈逐步减小的趋势，最终得到了拟合的函数

```sh
Result: y = -0.016946000556052058 + 0.8039868283288965*x + 0.00292346707858539*x**2 + -0.08582658982914065*x**3
```

对函数进行图形化描述，可以看到，其跟 $sin(x)$ 在给定的区间上是比较接近的，如图 4 所示。

![sinx](./pic/sin_x.png)

<center>图4  拟合曲线对比示意图</center>

