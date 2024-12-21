#include <iostream>
#include <omp.h>
using namespace std;

int main() {

    omp_set_num_threads(4);
    #pragma omp parallel
    {
        cout << "\nHello World from thread # " << omp_get_thread_num() << endl;
    }
    return 0;
}