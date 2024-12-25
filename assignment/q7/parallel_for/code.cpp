#include <iostream>
#include <omp.h>
#include <vector>
#include <chrono>
using namespace std;
using namespace chrono;

void printArray(vector<int> arr) {
    for (const auto& val : arr) {
        cout << val << " ";
    }
    cout << endl;
}

void initializeArray(vector<int>& arr, int N) {
    for (int i = 0; i < N; ++i) {
        arr[i] = i+1;
    }
}

int sequentialDotProduct(int N, vector<int>& arrA, vector<int>& arrB) {
    long long result = 0;

    auto start = high_resolution_clock::now(); 

    // Sequential summation
    for (int i = 0; i < N; i++) {
        result += arrA[i] * arrB[i];
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "Time taken (Sequential): " << elapsed.count() << " seconds" << endl;

    return result;

}

int parallelDotProduct(int N, vector<int>& arrA, vector<int>& arrB) {
    long long result = 0;

    auto start = high_resolution_clock::now(); 

    // Parallel summation
    #pragma omp parallel for reduction(+:result)
    for (int i = 0; i < N; i++) {
        result += arrA[i] * arrB[i];
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "Time taken (Parallel): " << elapsed.count() << " seconds" << endl;

    return result;

}

int main() {
    const int N = 10000; // Large array size

    vector<int> arrA(N, 0); // Initialize array A
    vector<int> arrB(N, 0); // Initialize array B
    initializeArray(arrA, N);
    initializeArray(arrB, N);

    // Sequential array sum
    long long sumSequential = sequentialDotProduct(N, arrA, arrB);
    printf("Sequential Dot Product: %lld\n", sumSequential);

    // Parallel array sum
    long long sumParallel = parallelDotProduct(N, arrA, arrB);
    printf("Parallel Dot Product: %lld\n", sumParallel);

    return 0;
}