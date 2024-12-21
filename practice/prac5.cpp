#include <iostream>
#include <omp.h>
using namespace std;

int main() {
    #pragma omp parallel for
    for (int i=0; i<5; i++) {
        cout << "Hello world!" << endl;
    }   
    return 0;
}