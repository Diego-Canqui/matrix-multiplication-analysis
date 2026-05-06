set terminal png
set output "grafica.png"

set title "Clasico vs Strassen"
set xlabel "n"
set ylabel "Tiempo (microsegundos)"
set grid
set key top left

plot "datos.dat" using 1:2 with linespoints title "Clasico", \
     "datos.dat" using 1:3 with linespoints title "Strassen"
