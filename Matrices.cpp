#include <iostream>

using namespace std;

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

	cout << "hola mundo" << endl;

	return 0;

}