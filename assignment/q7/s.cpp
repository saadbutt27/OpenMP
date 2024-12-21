#include <iostream>
#include <vector>

using namespace std;


void initialize_matrix(vector<vector<int>>& matrix, int N) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            matrix[i][j] = rand() % 10;
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
    const int N = 10000;
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

    // parallel_matrix_nultiplication(A, B, C, N);
    sequential_matrix_multiplication(A, B, C, N);

    // Print result matrix for verification
    cout << "Matrix C" << endl;
    printMatrix(C);

    return 0;
}