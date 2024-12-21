#include <iostream>
#include <omp.h>
using namespace std;

int main() {
    #pragma omp parallel sections num_threads(3)
    {
        #pragma omp section
        {
            int tid = omp_get_thread_num();
            #pragma omp critical 
            {
                cout << "X, tid = " << tid << endl; 
            }
        }
        #pragma omp section
        {
            int tid = omp_get_thread_num();
            #pragma omp critical 
            {
                cout << "Y, tid = " << tid << endl; 
            }
        }
        #pragma omp section
        {
            int tid = omp_get_thread_num();
            #pragma omp critical  
            {
                cout << "Z, tid = " << tid << endl; 
            }
        }
    }
    return 0;
}