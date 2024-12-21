#include <iostream>
#include <vector>

#include <omp.h>
using namespace std;

void parallel(int N) {
    int i, j;
    int arr[N];
    long long sum = 0;

    // Initialize the array
    for (i = 0; i < N; i++) {
        arr[i] = i + 1;
    }

    // Nested parallelism
    #pragma omp parallel shared(arr, sum) private(i, j)
    {
        #pragma omp for reduction(+:sum)
        for (i = 0; i < N; i++) {
            int temp_sum = 0;

            // Nested parallel region
            #pragma omp parallel for reduction(+:temp_sum) private(j)
            for (j = 0; j < N; j++) {
                temp_sum += arr[j] * arr[j];
            }

            // Critical section to update the global sum
            #pragma omp critical
            {
                sum += temp_sum;
            }
        }
    }

    cout << "Sum of squares: " << sum << endl;
}

void sequential(int N) {
    int i, j;
    int arr[N];
    long long sum = 0;

    // Initialize the array
    for (i = 0; i < N; i++) {
        arr[i] = i + 1;
        cout << arr[i] << " ";
    }
    cout << endl;

    // Calculate the sum of squares sequentially
    for (j = 0; j < N; j++) {
        sum += arr[j] * arr[j];
    }

    cout << "Sum of squares: " << sum << endl;
}


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
                    temp_sum += A[i][k] * B[k][j];
                }

                // Critical section to update matrix C
                #pragma omp critical
                {
                    C[i][j] += temp_sum;
                }
            }
        }
    }
}

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
    // sequential_matrix_multiplication(A, B, C, N);

    // Print result matrix for verification
    cout << "Matrix C" << endl;
    printMatrix(C);


    // cout << "Sequential algorithm: " << endl;
    // sequential();

    // cout << "Parallel algorithm: " << endl;
    // parallel();

    return 0;
}