#include <iostream>
#include <omp.h>
using namespace std;

int main() {
    int size = 100;
    int a[size], b[size], c[size];

    for (int i=0; i<size; i++) {
        a[i] = i;
        b[i] = i * 2;
    }

    #pragma omp parallel for num_threads(4)
    for (int i = 0; i < size; i++) {
        c[i] = a[i] + b[i];
        #pragma omp critical 
        {
            cout << "Iteration " << i << " c[" << i << "] = " << c[i] << endl;
        }
    }
    
    return 0;
}