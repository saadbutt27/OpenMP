#include <iostream>
#include <vector>
#include <chrono>

#include <omp.h>

using namespace std;
using namespace std::chrono;

void initialize_matrix(vector<vector<double>>& matrix, int N) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            matrix[i][j] = rand() % 10;
        }
    }
}

void printMatrix(vector<vector<double>> matrix) {
    for (const auto& row : matrix) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
}

void parallelAlgo(vector<vector<double>> A,
                  vector<vector<double>> B,
                  vector<vector<double>> C, int N) {

    auto start = high_resolution_clock::now();

    #pragma omp parallel shared(A, B, C, N)
    {
        // cout << "Outer parallel region, thread: " << omp_get_thread_num() << endl;
        #pragma omp for schedule(dynamic) // Parallel for loop for rows of C
        for (int i = 0; i < N; i++) {
            #pragma omp parallel for shared(A, B, C, N, i) // Nested parallel for loop for columns of C
            for (int j = 0; j < N; j++) {
                double sum = 0.0; // Private variable for each j
                #pragma omp parallel for reduction(+:sum) shared(A,B,N,i,j)
                for (int k = 0; k < N; k++) {
                    sum += A[i][k] * B[k][j];
                }
                #pragma omp critical // Critical section to update C (could use atomic if appropriate)
                {
                    C[i][j] = sum;
                }
            }
        }
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "Parallel Time: " << duration.count() << " ms" << endl;   

    cout << "Matrix C (Parallel)" << endl;
    // printMatrix(C);           

}

void sequentialAlgo(vector<vector<double>> &A,
                    vector<vector<double>> &B,
                    vector<vector<double>> &C, int N) {

    auto start = high_resolution_clock::now();

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            double sum = 0.0;
            for (int k = 0; k < N; k++) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "Sequential Time: " << duration.count() << " ms" << endl;

    cout << "Matrix C (Sequential)" << endl;
    // printMatrix(C);

}

int main() {
    int N = 10000;
    vector<vector<double>> A(N, vector<double>(N, 0.0));
    vector<vector<double>> B(N, vector<double>(N, 0.0));
    vector<vector<double>> C(N, vector<double>(N, 0.0));
    
    initialize_matrix(A, N);
    cout << "Matrix A" << endl;
    // printMatrix(A);
    initialize_matrix(B, N);
    cout << "Matrix B" << endl;
    // printMatrix(B);

    parallelAlgo(A, B, C, N);

    cout << "-------------------------------------" << endl;

    sequentialAlgo(A, B, C, N);

    return 0;
}