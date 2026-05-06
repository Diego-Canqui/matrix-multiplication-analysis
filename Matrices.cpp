#include <iostream>
#include <chrono>
#include <cstdlib>  //esta es para rand
#include <ctime>    //esto es para la semilla 
using namespace std;
using namespace chrono;

// ════════════════════════════════════════════════════════════════════════════
// LO QUE YO HICE - version original con arrays estaticos
// ════════════════════════════════════════════════════════════════════════════

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

// ════════════════════════════════════════════════════════════════════════════
// NUEVO - alias Matriz necesario para que Strassen sea recursivo.
// C++ no permite mezclar int[500][500] con int[512][512] aunque parezca
// lo mismo, por eso necesitamos este tipo separado.
// ════════════════════════════════════════════════════════════════════════════

const int MAX = 512;
typedef int Matriz[MAX][MAX];

// Version de multi_matriz_cuadrada que recibe Matriz en lugar de int[500][500]
// — misma logica de siempre, solo cambia el tipo para que Strassen pueda usarla
void multi_matriz_cuadrada(Matriz A, Matriz B, Matriz C, int n) {  //Esto es n^3
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = 0;
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// ── Operaciones auxiliares para Strassen ─────────────────────────────────────

// R = A + B
void sumar(Matriz A, Matriz B, Matriz R, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            R[i][j] = A[i][j] + B[i][j];
}

// R = A - B
void restar(Matriz A, Matriz B, Matriz R, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            R[i][j] = A[i][j] - B[i][j];
}

// Copia el bloque (filaInicio, colInicio) de tamaño n desde A hacia R
void extraer(Matriz A, Matriz R, int filaInicio, int colInicio, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            R[i][j] = A[i + filaInicio][j + colInicio];
}

// Pega la matriz R dentro de A en la posicion (filaInicio, colInicio)
void insertar(Matriz R, Matriz A, int filaInicio, int colInicio, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            A[i + filaInicio][j + colInicio] = R[i][j];
}

// ── Strassen recursivo (reutiliza mi funcion clasica como caso base) ──────────
void strassen(Matriz A, Matriz B, Matriz C, int n) {

    // Caso base: para n pequeño el clasico es mas eficiente
    if (n <= 64) {
        multi_matriz_cuadrada(A, B, C, n); // llama a mi funcion de arriba :)
        return;
    }

    int mitad = n / 2;

    static Matriz A11, A12, A21, A22;
    static Matriz B11, B12, B21, B22;

    extraer(A, A11, 0, 0, mitad);
    extraer(A, A12, 0, mitad, mitad);
    extraer(A, A21, mitad, 0, mitad);
    extraer(A, A22, mitad, mitad, mitad);

    extraer(B, B11, 0, 0, mitad);
    extraer(B, B12, 0, mitad, mitad);
    extraer(B, B21, mitad, 0, mitad);
    extraer(B, B22, mitad, mitad, mitad);

    // Los 7 productos de Strassen
    static Matriz P1, P2, P3, P4, P5, P6, P7;
    static Matriz temp1, temp2;

    // P1 = A11 * (B12 - B22)
    restar(B12, B22, temp1, mitad);
    strassen(A11, temp1, P1, mitad);

    // P2 = (A11 + A12) * B22
    sumar(A11, A12, temp1, mitad);
    strassen(temp1, B22, P2, mitad);

    // P3 = (A21 + A22) * B11
    sumar(A21, A22, temp1, mitad);
    strassen(temp1, B11, P3, mitad);

    // P4 = A22 * (B21 - B11)
    restar(B21, B11, temp1, mitad);
    strassen(A22, temp1, P4, mitad);

    // P5 = (A11 + A22) * (B11 + B22)
    sumar(A11, A22, temp1, mitad);
    sumar(B11, B22, temp2, mitad);
    strassen(temp1, temp2, P5, mitad);

    // P6 = (A12 - A22) * (B21 + B22)
    restar(A12, A22, temp1, mitad);
    sumar(B21, B22, temp2, mitad);
    strassen(temp1, temp2, P6, mitad);

    // P7 = (A11 - A21) * (B11 + B12)
    restar(A11, A21, temp1, mitad);
    sumar(B11, B12, temp2, mitad);
    strassen(temp1, temp2, P7, mitad);

    // Cuadrantes del resultado
    static Matriz C11, C12, C21, C22;

    // C11 = P5 + P4 - P2 + P6
    sumar(P5, P4, temp1, mitad);
    restar(temp1, P2, temp2, mitad);
    sumar(temp2, P6, C11, mitad);

    // C12 = P1 + P2
    sumar(P1, P2, C12, mitad);

    // C21 = P3 + P4
    sumar(P3, P4, C21, mitad);

    // C22 = P5 + P1 - P3 - P7
    sumar(P5, P1, temp1, mitad);
    restar(temp1, P3, temp2, mitad);
    restar(temp2, P7, C22, mitad);

    // Ensamblar los 4 cuadrantes en C
    insertar(C11, C, 0, 0, mitad);
    insertar(C12, C, 0, mitad, mitad);
    insertar(C21, C, mitad, 0, mitad);
    insertar(C22, C, mitad, mitad, mitad);
}

// ════════════════════════════════════════════════════════════════════════════
// MAIN
// ════════════════════════════════════════════════════════════════════════════
int main() {
    srand(time(NULL));

    // Mis arrays originales siguen aqui para el clasico
    static int A[500][500], B[500][500], C[500][500]; //se guarda en el data segment

    // Para Strassen necesito el tipo Matriz
    static Matriz A_m, B_m, C_strassen;

    const int REPETICIONES = 5;

    cout << "n" << "\t" << "clasico_us" << "\t" << "strassen_us" << endl;

    // Solo potencias de 2 (Strassen lo requiere)
    for (int n = 16; n <= 512; n *= 2) {

        llenar_matrices(A, B, n); // lleno mis arrays de siempre

        // Copio mis arrays a Matriz para que Strassen use los mismos datos
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) {
                A_m[i][j] = A[i][j];
                B_m[i][j] = B[i][j];
            }

        // Repetir mediciones sin volver a generar las matrices
        long long acum_clasico  = 0;
        long long acum_strassen = 0;

        for (int rep = 0; rep < REPETICIONES; rep++) {

            // Clasico - con mis arrays de siempre
            auto inicio = high_resolution_clock::now();
            multi_matriz_cuadrada(A, B, C, n);
            auto fin = high_resolution_clock::now();
            acum_clasico += duration_cast<microseconds>(fin - inicio).count();

            // Strassen - con los mismos datos copiados a Matriz
            auto inicio2 = high_resolution_clock::now();
            strassen(A_m, B_m, C_strassen, n);
            auto fin2 = high_resolution_clock::now();
            acum_strassen += duration_cast<microseconds>(fin2 - inicio2).count();
        }

        cout << n << "\t" << acum_clasico / REPETICIONES << "\t" << acum_strassen / REPETICIONES << endl;
    }

    return 0;
}