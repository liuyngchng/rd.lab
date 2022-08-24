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
splot besj0(x**2+y**2)' | gnuplot -persist
#exit 0
