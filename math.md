# 1. 方根的几何含义

（1）平方根（2次方根）：求2纬平面中面积为 $n$ 的正2边形的边长，其边长为$n$ 的2次方根 $\sqrt{n}$。

（2）立方根（3次方根）：求3维空间中体积为$n$ 的正立方体的边长，其边长为$n$ 的3次方根 $\sqrt[3]{n}$。

<img src="./img/root.svg" style="zoom:45%;" />

（3）扩展到 $n$ 次方根：求 $m$ 纬空间中正 $n$ 边形的边长，其边长为 $n$ 的 $m$ 次方根 $\sqrt[m]{n}$。

# 2. 导数

## 2.1 斜率

对于函数$y= f(x)$, 可能是个曲线（一纬空间为曲线，多维空间就是个多维曲面），那么说斜率的时候，就需要先指定位置，例如 $x = x'$的位置。因为斜率是随着 $x$ 的位置的不同在发生变化。

当自变量 $x$ 在 $x'$ 处增加 $\Delta x$ 时， 因变量 $y$ 的变化量为 $\Delta y$ ，斜率（在数学里叫**割线斜率**）为
$$
\frac{\Delta y}{\Delta x} = \frac{f(x+\Delta x)-f(x)}{\Delta x}
$$


  $x'$ 处的斜率为
$$
\frac{\Delta y}{\Delta x} = \frac{f(x'+\Delta x)-f(x')}{\Delta x}
$$
可见这里的斜率是平均变化率。

## 2.2 导数

导数指的是瞬时变化率， 即 $x$ 在 $x'$ 处当变化 $\Delta x$ 趋近于 0 时的平均变化率，函数 $y$ 对自变量 $x$ 求导， 记作
$$
\frac{dy}{dx}
$$
也计作
$$
f'(x)
$$
即：
$$
\frac{dy}{dx}= f'(x)=\lim_{\Delta x \to 0} f'(x') =\lim_{\Delta x \to 0}\frac{\Delta y}{\Delta x} = \lim_{\Delta x \to 0}\frac{f(x'+\Delta x)-f(x')}{\Delta x}
$$
接下来，推导一个 $ x^2$ 的导数计算过程，即  $y=f(x) = x^2$。
$$
\begin{align}
\frac{dy}{dx} = \frac{d(x^2)}{dx} =\lim_{\Delta x \to 0}\frac{\Delta y}{\Delta x} 
&= \lim_{\Delta x \to 0}\frac{(x+\Delta x)^2 - x^2}{\Delta x} \\
&= \lim_{\Delta x \to 0}\frac{x^2 + 2x\Delta x + (\Delta x)^2 - x^2}{\Delta x} \\
&= \lim_{\Delta x \to 0}\frac{2x\Delta x + (\Delta x)^2}{\Delta x} \quad \\
&= \lim_{\Delta x \to 0}(2x + \Delta x) \\
&= 2x \quad
\end{align}
$$


