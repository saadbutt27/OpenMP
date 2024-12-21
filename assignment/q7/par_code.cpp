#include <iostream>
#include <omp.h>
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
    // const int n = 100;
    // int arr[n];
    // int sumOfSquares = 0;

    // // Initializing the array
    // for (int i=0; i<n; i++) {
    //     arr[i] = i+1;
    // }

    // // Parallely computing sum of squares of the array
    // #pragma omp parallel num_threads(4)
    // {
    //     int tid = omp_get_thread_num(); // thread id
    //     int localSum = 0; // local/private variable for each thread

    //     // Nested parallelism (inner loop)
    //     #pragma omp parallel for reduction(+:localSum)
    //     for (int i=0; i<n; i++) {
    //         localSum += (arr[i] * arr[i]);
    //     }

    //     // Critical section for adding each thread's local sum to shared sumOfSquares variable
    //     #pragma omp critical
    //     {
    //         sumOfSquares += localSum;
    //     }

    //     #pragma omp single
    //     {
    //         cout << "Thread no. " << tid << " local sum of sqaures = " << localSum << endl;
    //     }
    // }

    // cout << "Parallel - Sum of Squares of the array = " << sumOfSquares << endl;

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

    #pragma omp parallel collapse(2) shared(matrixA, matrixB, matrixC) num_threads(4)
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            for (int k=0; k<n; k++) {
                #pragma omp critical
                matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
            }
        }
    } 
    cout << "C" << endl;
    printMatrix(matrixC, n);
    
    return 0;
}