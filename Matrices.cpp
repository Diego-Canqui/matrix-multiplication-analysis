#include <iostream>
#include <chrono>
#include <cstdlib>  //esta es para rand
#include <ctime>    //esto es para la semilla 

using namespace std;
using namespace chrono;

//Recordando la primera vez que hice multiplicacion de matrices
void multi_matriz_cuadrada(int A[500][500], int B[500][500], int C[500][500], int n) {  //Esto es n^3
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			C[i][j] = 0;
			for (int k = 0; k < n; k++) {
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}
}

// Inicializar matrices con valores aleatorios
void llenar_matrices(int A[500][500], int B[500][500], int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            A[i][j] = rand() % 100;
            B[i][j] = rand() % 100;
        }
}

int main() {

    srand(time(NULL));

    static int A[500][500], B[500][500], C[500][500]; //se guarda en el data segment

    cout << "n" << "\t" << "tiempo_us" << endl;
    for (int n = 20; n <= 300; n += 10) {
        llenar_matrices(A, B, n);
        auto inicio = high_resolution_clock::now();
        multi_matriz_cuadrada(A, B, C, n);
        auto fin = high_resolution_clock::now();
        auto duracion = duration_cast<microseconds>(fin - inicio);
        cout << n << "\t" << duracion.count() << endl;
    }

    return 0;
}