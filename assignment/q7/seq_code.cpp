#include <iostream>
using namespace std;

void printMatrix(int matrix[][3], int n) {
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    } 
}

int main() {
    // const int n = 10;
    // int arr[n];
    // int sumOfSquares = 0;

    // // Initializing the array
    // for (int i=0; i<n; i++) {
    //     arr[i] = i+1;
    // }

    // // Sequentially computing sum of squares of the array
    // for (int i=0; i<n; i++) {
    //     sumOfSquares += (arr[i] * arr[i]);
    // }

    // cout << "Sequential - Sum of Squares of the array = " << sumOfSquares << endl;

    const int n=3;
    int matrixA[n][n], matrixB[n][n], matrixC[n][n] = {0};

    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            matrixA[i][j] = i+j+1;
            matrixB[i][j] = i-j+1;
        }
    } 

    cout << "A" << endl;
    printMatrix(matrixA, n);
    cout << "B" << endl;
    printMatrix(matrixB, n);
    
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            for (int k=0; k<n; k++) {
                matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
            }
        }
    } 
    cout << "C" << endl;
    printMatrix(matrixC, n);

    return 0;
}