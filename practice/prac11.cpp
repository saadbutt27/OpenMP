#include <iostream>
#include <omp.h>
using namespace std;

#define NT 4
#define thrd_no omp_get_thread_num

int main() {
    #pragma omp parallel for num_threads(NT)
    for (int i=0; i<NT; i++) cout << "thread no: " << thrd_no() << endl;

    #pragma omp parallel for \
                    num_threads(NT)
    for (int i=0; i<NT; i++) cout << "thread no: " << thrd_no() << endl;
    
    #pragma omp parallel num_threads(NT)
    #pragma omp for
    for (int i=0; i<NT; i++) cout << "thread no: " << thrd_no() << endl;
    
    #pragma omp parallel num_threads(NT)
    {
        int no = thrd_no();
        if (no % 2) { cout << "thread no " << no << " is Odd" << endl; }
        else { cout << "thread no " << no << " is Even" << endl; }

        #pragma omp for
        for (int i=0; i<NT; i++) cout << "thread no: " << thrd_no() << endl;

    }

    return 0;
}