#include <iostream>
#include <chrono>

using namespace std;
using namespace chrono;

//Recordando la primera vez que hice multiplicacion de matrices
void multi_matriz_cuadrada(int A[4][4], int B[4][4], int C[4][4], int n) {  //Esto es n^3
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			C[i][j] = 0;
			for (int k = 0; k < n; k++) {
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}
}

int main() {

    int n = 4; //variable del tamanio 

    int A[4][4], B[4][4], C[4][4];

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            A[i][j] = 1;
            B[i][j] = 1;
        }

    auto inicio = high_resolution_clock::now();

    multi_matriz_cuadrada(A, B, C, n);

    auto fin = high_resolution_clock::now();

    auto duracion = duration_cast<microseconds>(fin - inicio);

    cout << "Tiempo: " << duracion.count() << " microsegundos\n";

    return 0;
}