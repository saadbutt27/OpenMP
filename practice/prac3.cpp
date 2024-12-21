#include <iostream>
#include <omp.h>
using namespace std;

int main() {

    #pragma omp parallel num_threads(6)
    {
        cout << "\nHello World from thread # " << omp_get_thread_num() << endl;
    }
    return 0;
}