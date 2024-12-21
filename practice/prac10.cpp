#include <iostream>
#include <algorithm>
#define ARRAY_SIZE 100000000
#define ARRAY_VALUE 1231
int main()
{
    int *arr = new int[ARRAY_SIZE];
    std::fill_n(arr, ARRAY_SIZE, ARRAY_VALUE);
    #pragma omp parallel
    {
        #pragma omp for
        for(int i = 0; i < ARRAY_SIZE; i++) {
            // do some relatively long operation
            arr[i] = arr[i] / arr[i] + arr[i] / 5 - 14;
        }
    }
    return 0;
}