#include <iostream>
#include <omp.h>
using namespace std;

int main() {
    int x = 0;
    #pragma omp parallel num_threads(300)
    {
        // #pragma omp critical
        {
            x = x+1;
        }
    }
    cout << "x = " << x << endl;
    return 0;
}