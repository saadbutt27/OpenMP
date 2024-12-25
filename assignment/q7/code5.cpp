#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

#define SIZE 3

void printMatrix(vector<vector<int>> matrix) {
    for (const auto& row : matrix) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
}

void matrix_multiply(const vector<vector<int>>& A, 
                     const vector<vector<int>>& B, 
                     vector<vector<int>>& C) {

    omp_set_nested(1); // Enable nested parallelism
    #pragma omp parallel for shared(A, B, C) // Outer loop parallelism
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            int sum = 0;

            #pragma omp parallel for reduction(+:sum) // Inner loop parallelism with reduction
            for (int k = 0; k < SIZE; k++) {
                sum += A[i][k] * B[k][j];
            }

            C[i][j] = sum; // Assign the reduced value to the result matrix
        }
    }
}

int main() {
    // Initialize matrices A and B
    vector<vector<int>> A(SIZE, vector<int>(SIZE, 1));
    vector<vector<int>> B(SIZE, vector<int>(SIZE, 2));
    vector<vector<int>> C(SIZE, vector<int>(SIZE, 0));

    printf("Matrix A\n");
    printMatrix(A);

    printf("Matrix B\n");
    printMatrix(B);

    matrix_multiply(A, B, C);

    printf("Matrix C\n");
    printMatrix(C);

    return 0;
}
