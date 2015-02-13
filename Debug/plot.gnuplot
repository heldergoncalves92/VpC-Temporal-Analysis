
set terminal png
set out "alpha.png"
set title "Histograma"
set xlabel "N Frames"
set ylabel "Alpha"
set grid


set style data linespoints

f(x) = exp(-x**2 / 2)



#plot [t=0:4] f(t) 
#PLOT ALPHA
set yrange [-1 : 0]
plot "register.data"



#PLOT SLOP DERIVATE
set out "derivate.png"
set yrange [-0.5 : 1]
plot "derivate.data"