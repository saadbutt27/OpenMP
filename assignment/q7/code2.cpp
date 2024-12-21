#include <iostream>
#include <vector>

#include <omp.h>
using namespace std;

// Function for matrix multiplication using OpenMP
void parallel_matrix_multiplication(const vector<vector<int>>& A, 
                                     const vector<vector<int>>& B, 
                                     vector<vector<int>>& C, int N) {
    int sum = 0; // Private variable for reduction
    
    omp_set_nested(1); // Enable nested parallelism

    #pragma omp parallel shared(A, B, C, N)
    {
        // Parallel for loop over rows of the first matrix
        #pragma omp for
        for (int i = 0; i < N; ++i) {
            // Nested parallelism for each row's column computation
            #pragma omp parallel shared(A, B, C) private(i)
            {
                #pragma omp for
                for (int j = 0; j < N; ++j) {
                    // Reduction to compute dot product
                    #pragma omp for reduction(+:sum)
                    for (int k = 0; k < N; ++k) {
                        sum += A[i][k] * B[k][j];
                    }
                    // Update the result matrix inside a critical section
                    #pragma omp critical
                    {
                        C[i][j] = sum;
                    }
                }
            }
        }
    }
}

// Function for matrix multiplication sequentially
void sequential_matrix_multiplication(const vector<vector<int>>& A, 
                                       const vector<vector<int>>& B, 
                                       vector<vector<int>>& C, int N) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            C[i][j] = 0; // Initialize the result matrix cell
            for (int k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void printMatrix(char matrixName, vector<vector<int>> matrix) {
    cout << "Matrix " << matrixName << ": " << endl;
    for (const auto& row : matrix) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
}

int main() {
    int N = 3; // Size of the square matrices
    vector<vector<int>> A(N, vector<int>(N, 1));
    vector<vector<int>> B(N, vector<int>(N, 2));
    vector<vector<int>> C(N, vector<int>(N, 0));

    printMatrix('A', A);
    printMatrix('B', B);

    // sequential_matrix_multiplication(A, B, C, N);

    parallel_matrix_multiplication(A, B, C, N);

    cout << "Resultant ";
    printMatrix('C', C);

    return 0;
}
