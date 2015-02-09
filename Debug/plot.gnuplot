
set terminal png
set out "print.png"
set title "Some Sample Plots"
set xlabel "Independent Variable (no units)"
set ylabel "Dependent Variable (no units)"
set grid

set style data linespoints

f(x) = exp(-x**2 / 2)



#plot [t=0:4] f(t) 
#plot [0:50] "register.data"
plot "register.data"