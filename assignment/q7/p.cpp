#include <iostream>
#include <vector>

#include <omp.h>
using namespace std;


void initialize_matrix(vector<vector<int>>& matrix, int N) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            matrix[i][j] = rand() % 10;
        }
    }
}

void parallel_matrix_nultiplication(const vector<vector<int>>& A, 
                                    const vector<vector<int>>& B, 
                                    vector<vector<int>>& C, int N) {
    // Nested parallelism
    #pragma omp parallel shared(A, B, C)
    {
        // Get the thread number for the outer parallel region
        int outer_thread_num = omp_get_thread_num();
        
        // Use a critical section to avoid interleaved prints
        #pragma omp critical
        {
            cout << "Outer parallel region, thread: " << outer_thread_num << endl;
        }

        // Parallel for loop for matrix multiplication
        int i, j;
        #pragma omp for private(i, j)
        for (i = 0; i < N; ++i) {
            for (j = 0; j < N; ++j) {
                int temp_sum = 0;

                // Nested parallel region for inner loop
                int k;
                #pragma omp parallel for reduction(+:temp_sum) private(k)
                for (k = 0; k < N; ++k) {
                    int inner_thread_num = omp_get_thread_num();
                    
                    // Critical section to ensure one thread prints at a time
                    #pragma omp critical
                    {
                        cout << outer_thread_num << " Inner loop, thread: " << inner_thread_num 
                             << ", calculating A[" << i << "][" << k << "] * B[" << k << "][" << j 
                             << "] = " << A[i][k] * B[k][j] << endl;
                    }

                    temp_sum += A[i][k] * B[k][j];
                }

                // Critical section to update matrix C
                #pragma omp critical
                {
                    cout << "Critical section, thread: " << omp_get_thread_num()
                         << ", updating C[" << i << "][" << j << "] += " << temp_sum << endl;
                    C[i][j] += temp_sum;
                }
            }
        }
    }
}

void printMatrix(vector<vector<int>> matrix) {
    for (const auto& row : matrix) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
}

int main() {
    const int N = 3;
    vector<vector<int>> A(N, vector<int>(N));
    vector<vector<int>> B(N, vector<int>(N));
    vector<vector<int>> C(N, vector<int>(N, 0));

    // Initialize matrices A and B
    initialize_matrix(A, N);
    cout << "Matrix A" << endl;
    printMatrix(A);

    initialize_matrix(B, N);
    cout << "Matrix B" << endl;
    printMatrix(B);

    parallel_matrix_nultiplication(A, B, C, N);

    // Print result matrix for verification
    cout << "Matrix C" << endl;
    printMatrix(C);

    return 0;
}