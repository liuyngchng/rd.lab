#  setup

```shell
sudo apt-get install gnuplot
gnuplot
```
#  2D plot

```shell
gnuplot
plot[-3.14: 3.14] sin(x), cos(x)
```
#  3D plot

```sh
#!/bin/bash
echo '
reset
set isosamples 40
unset key
set hidden3d
set title "J_0(r^2)"
# 设置x轴取值范围
set xrange [-5:5]
# 设置 y轴取值范围
set yrange [-5:5]
set ztics 1
set pm3d
splot besj0(x**2+y**2)' | gnuplot

```

```sh
set samples 25
set isosamples 26
set title "Test 3D gnuplot"
set contour base
set hidden3d offset 1
splot [-12:12.01] [-12:12.01] sin(sqrt(x**2+y**2))/sqrt(x**2+y**2)
```





# 数据文件画图

##  生成散点数据文件

```shell
touch data.dat
vi data.dat				# 第 0 列为 x， 第 1 列为 y
1, 2
2, 4
3, 9
```

##  绘制散点数据文件

````shell
gnuplot
file = "data.dat"
plot file using 0:1 titile "my data"  # 指定第 0 列为 x ，第 1 列为 y
````

##  画多个数据文件

```sh
gnuplot>set multiplot 			% 设置为多图模式
gnuplot>set origin 0.0,0.0 		% 设置第一个图的原点的位置
gnuplot>set size 0.5,0.5 		% 设置第一个图的大小
gnuplot>plot “a1.dat”
gnuplot>set origin 0.0,0.5 		% 设置第二个图的原点的位置
gnuplot>set size 0.5,0.5 		% 设置第二个图的大小
gnuplot>plot “a2.dat”
gnuplot>set origin 0.0,0.0 		% 设置第三个图的原点的位置
gnuplot>set size 0.5,0.5 		% 设置第三个图的大小
gnuplot>plot “a3.dat”
gnuplot>set origin 0.0,0.0 		% 设置第四个图的原点的位置
gnuplot>set size 0.5,0.5 		% 设置第四个图的大小
```



# 画多个图

```sh
set yrange [-2:2]
plot[0:7] sin(x), cos(x)
```

# 定义函数

定义3D函数，并画图

```sh
gnuplot> 
f(x,y)=sin (sqrt(x∗x+y∗y) )/sqrt(x∗x+y∗y)
splot f (x,y)
```

定义2D函数，并画图

```sh
gnuplot>
f(x)=sin(x)
plot f(x)
```

$$
[
\delta_t = \frac{\partial L}{\partial y_t}
]
$$

