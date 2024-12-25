#include <iostream>
#include <omp.h>
#include <vector>
#include <chrono>
using namespace std;
using namespace chrono;

void printMatrix(vector<vector<double>> matrix) {
    for (const auto& row : matrix) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
}

void initializeMatrix(vector<vector<double>>& matrix, int N) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            matrix[i][j] = rand() % 10 + i;
        }
    }
}

// Function to generate a submatrix by excluding row 0 and column `col`
vector<vector<double>> getSubMatrix(const vector<vector<double>>& matrix, int col) {
    int n = matrix.size();
    vector<vector<double>> subMatrix(n - 1, vector<double>(n - 1));

    for (int i = 1; i < n; ++i) { // Start from row 1
        int subCol = 0;
        for (int j = 0; j < n; ++j) {
            if (j == col) continue; // Skip the column
            subMatrix[i - 1][subCol] = matrix[i][j];
            ++subCol;
        }
    }
    return subMatrix;
}

double parallelDeterminant(const vector<vector<double>>& matrix) {
    int n = matrix.size();
    if (n == 1) return matrix[0][0]; // Base case for 1x1 matrix

    double det = 0.0;
    int j;
    #pragma omp parallel for reduction(+:det) shared(matrix) private(j)
    for (j = 0; j < n; ++j) {
        // Generate submatrix for each thread
        vector<vector<double>> subMatrix = getSubMatrix(matrix, j);
        double subDet = parallelDeterminant(subMatrix); // Parallel recursion
        det += (j % 2 == 0 ? 1 : -1) * matrix[0][j] * subDet;
    }
    return det;
}

double sequentialDeterminant(const vector<vector<double>>& matrix) {
    int n = matrix.size();
    if (n == 1) return matrix[0][0]; // Base case for 1x1 matrix

    double det = 0.0;
    for (int j = 0; j < n; ++j) {
        vector<vector<double>> subMatrix = getSubMatrix(matrix, j);
        det += (j % 2 == 0 ? 1 : -1) * matrix[0][j] * sequentialDeterminant(subMatrix);
    }
    return det;
}

int main() {
    const int N = 10; // Large array size

    // vector<vector<double>> matrix = {
    //     {2, -3, 1},
    //     {2, 0, -1},
    //     {1, 4, 5}
    // };

    vector<vector<double>> matrix(N, vector<double>(N, 0)); // Initialize array with 1's
    initializeMatrix(matrix, N);
    printMatrix(matrix);

    // Sequential matrix Determinant
    cout << "Sequential Determinant: " << sequentialDeterminant(matrix) << endl;

    // Parallel matrix Determinant
    cout << "Parallel Determinant: " << parallelDeterminant(matrix) << endl;

    return 0;
}