#include <iostream>
#include <omp.h>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

void printMatrix(vector<vector<double>> matrix) {
    for (const auto& row : matrix) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
}

void initialize_matrix(vector<vector<double>>& matrix, int N) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            matrix[i][j] = rand() % 10;
        }
    }
}

void matrixMultiplySequential(const int N, vector<vector<double>>& A,
                                           vector<vector<double>>& B,
                                           vector<vector<double>>& C) {
    
    auto start = high_resolution_clock::now();                                        

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            C[i][j] = 0; // Initialize the result
            for (int k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "Sequential Time: " << duration.count() << " ms" << endl; 
}

void matrixMultiplyParallel(const int N, vector<vector<double>>& A,
                                         vector<vector<double>>& B,
                                         vector<vector<double>>& C) {
    
    auto start = high_resolution_clock::now();

    omp_set_nested(1); // Enable nested parallelism

    #pragma omp parallel for // Outer parallel region
    for (int i = 0; i < N; ++i) {
        int outer_thread_id = omp_get_thread_num();
        #pragma omp parallel for // Inner parallel region
        for (int j = 0; j < N; ++j) {
            int inner_thread_id = omp_get_thread_num();
            C[i][j] = 0; // Initialize the result
            for (int k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "Parallel Time: " << duration.count() << " ms" << endl;
}

int main() {
    const int N = 1000;
    vector<vector<double>> A(N, vector<double>(N));
    vector<vector<double>> B(N, vector<double>(N));
    vector<vector<double>> C(N, vector<double>(N));

    // Initialize matrices A and B
    initialize_matrix(A, N);
    initialize_matrix(B, N);

    // cout << "Matrix A and B are: " << endl;
    // printMatrix(A);
    // printMatrix(B);

    // Priniting matrice A and B respectibely
    matrixMultiplySequential(N, A, B, C);
    // cout << "Matrix C (Sequential): " << endl;
    // printMatrix(C);

    // Perform sequential matrix multiplication
    matrixMultiplyParallel(N, A, B, C);
    // cout << "Matrix C (Parallel): " << endl;

    return 0;
}