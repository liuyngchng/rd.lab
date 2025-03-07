# 多边形面积

中心点到正多边形的距离为r，正多边形有n个边，设边长为l。则面积a
$$
a= n \cdot \frac{1}{2}lr\tag{1}
$$
其中 $r$ 和 $l$可以用三角函数计算出来，即
$$
\begin{align}
tan(\frac{360^o}{2n})&=\frac{l}{2r}\\
l&= 2r\cdot tan(\frac{360^o}{2n})\tag{2}
\end{align}
$$
（2）带入（1）得到：
$$
\begin{align}
a&=n\cdot\frac{1}{2}[2r\cdot tan(\frac{360^o}{2n})]r\\
&=nr^2tan(\frac{180^o}{n})\\
&=r^2n\cdot tan(\frac{180^o}{n})\\

\end{align}
$$
