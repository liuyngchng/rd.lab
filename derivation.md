# 1. 求导

## 1.1 平方求导

（1）已知：
$$
y=f(x) \\
f(x)=x^2+1
$$
（2）求解：
$$
\frac{dy}{dx} =f'(x)= \lim_{\Delta x \to 0}{\frac{\Delta y}{\Delta x}}
$$
（3）解答
$$
\begin{align}
\frac{\Delta y}{\Delta x} &= \frac{f(x+\Delta x)-f(x)}{\Delta x}\\
&=\frac{[(x+\Delta x)^2 + 1]-[x^2+1]}{\Delta x} \\
&= \frac{(x^2 + 2\Delta x*x + \Delta x^2 +1)-(x^2+1)}{\Delta x}\\
&=\frac{2\Delta x \cdot x + \Delta x^2}{\Delta x} \\ 
&= 2x+\Delta x
\end{align}
$$


$$
\begin{align}
\frac{dy}{dx} &= \lim_{\Delta x \to 0}\frac{\Delta y}{\Delta x}\\ 
&= \lim_{\Delta x \to 0}(2x+\Delta x) \\
&= 2x
\end{align}
$$


## 1.2 开方求导
（1）已知：

$$
y=f(x)\newline f(x)=\sqrt{x}
$$
（2）求解：
$$
f'(x) = ?
$$


（3）解答：
$$
\begin {align}
\frac{\Delta y}{\Delta x} &= \frac{f(x+\Delta x)-f(x)}{\Delta x}\\\\
&=\frac{\sqrt{x+\Delta x}-\sqrt{x}}{\Delta x}
\end {align}
$$
两边都乘以 $y$ , 换算一下
$$
\begin{align}
\frac{\Delta y}{\Delta x}\cdot y &=\frac{\sqrt{x+\Delta x }- \sqrt{x}}{\Delta x}\cdot \sqrt{x}\\\\
&=\frac{\sqrt{x+\Delta x}\cdot\sqrt{x}- x}{\Delta x}\\\\
&=\frac{\sqrt{x^2+\Delta x\cdot x}- x}{\Delta x}\\\\
&=\frac{(\sqrt{x^2+\Delta x\cdot x}- x)(\sqrt{x^2+\Delta x\cdot x}+ x)}{\Delta x(\sqrt{x^2+\Delta x\cdot x}+x)}\\\\
&=\frac{(\sqrt{x^2+\Delta x\cdot x})^2- x^2}{\Delta x(\sqrt{x^2+\Delta x\cdot x}+x)}\\\\
&=\frac{(x^2+\Delta x\cdot x)- x^2}{\Delta x(\sqrt{x^2+\Delta x\cdot x}+x)}\\\\
&=\frac{\Delta x\cdot x}{\Delta x(\sqrt{x^2+\Delta x\cdot x}+x)}\\\\
&=\frac{ x}{\sqrt{x^2+\Delta x\cdot x}+x}\\\\
\end{align}
$$
变换一下, 将$y$ 移动到等式右侧，
$$
\begin{align}
\frac{\Delta y}{\Delta x} &=\frac{ x}{\sqrt{x^2+\Delta x\cdot x}+x}\cdot \frac{1}{y}\\\\
&=\frac{x}{\sqrt{x^2+\Delta x\cdot x}+x}\cdot \frac{1} {\sqrt{x}}\\\\
&=\frac{x}{(\sqrt{x^2+\Delta x\cdot x}+x)\sqrt{x}}\\\\
&=\frac{x}{\sqrt{x^3+\Delta x\cdot x^2}+x\sqrt{x}}\\\\
\end{align}
$$
求导，
$$
\begin{align}
\frac{dy}{dx}&=\lim_{\Delta x \to 0}{\frac{\Delta y}{\Delta x}}\\\\
&=\lim_{\Delta x \to 0}{\frac{x}{\sqrt{x^3+\Delta x\cdot x^2}+x\sqrt{x}}}\\\\
&=\frac{x}{\sqrt{x^3}+x\sqrt{x}}\\\\
&=\frac{x}{x\sqrt{x}+x\sqrt{x}}\\\\
&=\frac{x}{2x\sqrt{x}}\\\\
&=\frac{1}{2\sqrt{x}}\\
\end{align}
$$

