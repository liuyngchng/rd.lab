# 1. setup

```shell
sudo apt-get install gnuplot
gnuplot
```
# 2. 2D plot

```shell
gnuplot
plot[-3.14: 3.14] sin(x), cos(x)
```
# 3. 3D plot

```shell
#!/bin/bash
echo '
reset
set isosamples 40
unset key
set hidden3d
set title "J_0(r^2)"
set xrange [-5:5]
set yrange [-5:5]
set ztics 1
set pm3d
splot besj0(x**2+y**2)' | gnuplot

```

# 散点图

## 生成散点数据文件

```shell
touch data.dat
vi data.dat				# 第 0 列为 x， 第 1 列为 y
1, 2
2, 4
3, 9
```

## 绘制散点数据文件

````shell
gnuplot
file = "data.dat"
plot file using 0:1 titile "my data"  # 指定第 0 列为 x ，第 1 列为 y
````

