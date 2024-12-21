#include <iostream>
#include <vector>
#include <limits>

#include <omp.h>

using namespace std;

void parallel_algorithm(int N) {
    vector<int> squares(N + 1); // Shared array for storing squares
    int max_value = INT_MIN;   // Shared variable for the maximum value
    long long sum = 0;         // Shared variable for reduction

    // Parallel for loop to calculate squares
    #pragma omp parallel for shared(squares)
    for (int i = 1; i <= N; i++) {
        squares[i] = i * i;
    }

    // Single parallel region for reduction and max calculation
    #pragma omp parallel shared(sum, max_value, squares)
    {
        // Reduction for sum calculation
        #pragma omp for reduction(+:sum)
        for (int i = 1; i <= N; i++) {
            sum += squares[i];
        }

        // Finding the maximum value using a critical section
        #pragma omp for
        for (int i = 1; i <= N; i++) {
            #pragma omp critical
            {
                if (squares[i] > max_value) {
                    max_value = squares[i];
                }
            }
        }
    }

    cout << "Parallel Algorithm: Sum = " << sum 
         << ", Max = " << max_value << endl;
}

void sequential_algorithm(int N) {
    vector<int> squares(N + 1);
    int max_value = INT_MIN;
    // int max_value = numeric_limits<int>::min();
    long long sum = 0;

    // Calculate squares
    for (int i = 1; i <= N; i++) {
        squares[i] = i * i;
    }

    // Calculate sum and find maximum
    for (int i = 1; i <= N; i++) {
        sum += squares[i];
        if (squares[i] > max_value) {
            max_value = squares[i];
        }
    }

    cout << "Sequential Algorithm: Sum = " << sum 
         << ", Max = " << max_value << endl;
}

int main() {

    // Sequential algortihm 
    sequential_algorithm(10);

    // Parallel algortihm 
    parallel_algorithm(10);

    return 0;
}