#include <iostream>
#include <omp.h>
using namespace std;

int main() {
    int x = 0;

    omp_lock_t writelock;

    omp_init_lock(&writelock);
    
    #pragma omp parallel num_threads(300)
    {
        omp_set_lock(&writelock);
        x++;
        omp_unset_lock(&writelock);
    }
    cout << "x = " << x << endl;

    omp_destroy_lock(&writelock);
    return 0;
}