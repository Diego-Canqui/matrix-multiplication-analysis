all: plot

matrices: Matrices.cpp
	g++ Matrices.cpp -o matrices

datos.dat: matrices
	./matrices > datos.dat

plot: datos.dat script.gp
	gnuplot script.gp

clean:
	rm -f matrices datos.dat
