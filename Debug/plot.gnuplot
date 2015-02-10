
set terminal png
set out "alpha.png"
set title "Histograma"
set xlabel "N Frames"
set ylabel "Alpha"
set grid

set style data linespoints

f(x) = exp(-x**2 / 2)



#plot [t=0:4] f(t) 
#plot [0:50] "register.data"
plot "register.data"

set out "derivate.png"
plot "derivate.data"