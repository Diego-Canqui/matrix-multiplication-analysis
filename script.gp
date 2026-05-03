set terminal png
set output "grafica.png"

set title "Multiplicacion de matrices"
set xlabel "n"
set ylabel "Tiempo (microsegundos)"
set grid

plot "datos.dat" using 1:2 with linespoints title "Matrices"
