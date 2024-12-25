#include <iostream>
#include <omp.h>
#include <vector>
#include <chrono> // For measuring execution time

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

int sequentialArraySum(int n, vector<int>& arr) {
    long long sum = 0; // Variable for product

    auto start = high_resolution_clock::now(); 

    for (int i = 0; i < n; ++i) {
        sum += arr[i] * arr[i];
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "Time taken (Sequential): " << elapsed.count() << " seconds" << endl;

    return sum;

}

int parallelArraySum(int n, vector<int>& arr) {
    int sum = 0; // Shared variable for product

    auto start = high_resolution_clock::now(); 

    #pragma omp parallel
    {
        #pragma omp for
        for (int i = 0; i < n; ++i) {
            long long square = arr[i] * arr[i];

            #pragma omp critical 
            {
                sum += square; // Critical section to safely update the shared sum
            }
        }

    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "Time taken (Parallel): " << elapsed.count() << " seconds" << endl;

    return sum;

}

int main() {
    int N = 1000; // Array size
    vector<int> arr(N, 1); // Initialize array
    initializeArray(arr, N);

    // Sequential array sum
    long long sumSequential = sequentialArraySum(N, arr);
    printf("Sequential Sum: %lld\n", sumSequential);

    // Parallel array sum
    long long sumParallel = parallelArraySum(N, arr);
    printf("Parallel Sum: %lld\n", sumParallel);

    return 0;
}
