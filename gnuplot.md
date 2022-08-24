# 1. setup

```
sudo apt-get install gnuplot
gnuplot
```
# 2. 2D plot

```
gnuplot
plot[-3.14: 3.14] sin(x), cos(x)
```
# 3. 3D plot

```
reset
set isosamples 40
unset key
set hidden3d
set title "J_0(r^2)"
set xrange [-5:5]
set yrange [-5:5]
set ztics 1
set pm3d
splot besj0(x**2+y**2)

```
