#include <iostream>
#include <omp.h>
using namespace std;

void simple(int n, float *a, float *b) {
    int i;

    cout << "Parallel region starts" << endl;
    #pragma omp parallel
        #pragma omp single
            cout << "Total threads: " << omp_get_num_threads() << endl;
    
    #pragma omp parallel for 
        for (i=1; i<n; i++) { /** i is private by default */
            b[i] = (a[i] + a[i-1]) / 2.0;
            #pragma omp critical
                cout << "thread no: " << omp_get_thread_num() << " " << b[i] << endl;
        }
    cout << "Parallel region ends" << endl;
}

void subDomain(float *x, int istart, int ipoints) {
    // This function will write a value to the specified indices of the array executing in a thread
    cout << "istart: " << istart << " ipoints: " << ipoints << endl;
    for (int i=0; i < ipoints; ++i) {
        x[istart+i] = 123.456;
    }
}

void sub(float *x, int npoints) {
    // This function will distribute the toal array into the available threads
    int iam, nt, ipoints, istart;

    #pragma omp parallel default(shared) private(iam, nt, ipoints, istart)
    {
        iam = omp_get_thread_num(); // Thread ID (0 to nt-1)
        cout << "Thread ID: " << iam;
        nt = omp_get_num_threads(); // Total number of threads
        cout << "Total threads: " << nt << endl;
        ipoints = npoints / nt; // size of partition / Number of points each thread will process
        cout << "Size of partition for each thread: " << ipoints << endl;
        istart = iam * ipoints; // starting array index  / Starting index for this thread
        cout << "Starting index for this thread: " << istart << endl;

        // Last thread takes care of remaining elements
        if (iam == nt-1) //last thread may do more
            ipoints = npoints - istart;

        // Call subDomain to process this thread's partition
        subDomain(x, istart, ipoints);
    }
}

void nested_parallel_example() {
    int outer_threads, inner_threads;

    // Enable nested parallelism
    omp_set_nested(1);

    #pragma omp parallel private(outer_threads)
    {
        #pragma omp critical
        {
            outer_threads = omp_get_thread_num();

            cout << "Outer Thread ID: " << outer_threads << " in Level 1" << endl;

        }

        // Inner parallel region
        #pragma omp parallel private(inner_threads)
        {
            #pragma omp critical
            {
                inner_threads = omp_get_thread_num();
                cout << "    Inner Thread ID: " << inner_threads 
                    << " (Outer Thread ID: " << outer_threads << ")" << endl;
            }
        }
    }
}


void nested_parallel_example2() {
    omp_set_nested(1); // Enable nested parallelism

    #pragma omp parallel num_threads(2) // Outer region: 2 threads
    {
        int outer_thread_id = omp_get_thread_num();
        cout << "Outer Thread ID: " << outer_thread_id << " in Level 1" << endl;

        #pragma omp parallel num_threads(3) // Inner region: 3 threads
        {
            int inner_thread_id = omp_get_thread_num();
            cout << "    Inner Thread ID: " << inner_thread_id 
                 << " (Outer Thread ID: " << outer_thread_id << ")" << endl;
        }
    }
}


void parallel_for_loop() {
    const int size = 10;
    int array[size];

    // Initialize the array with zeros
    for (int i = 0; i < size; i++) {
        array[i] = 0;
    }

    // Parallel for loop
    #pragma omp parallel for num_threads(6)
    for (int i = 0; i < size; i++) {
        array[i] = i * i; // Calculate square of each index
        #pragma omp critical
        {
            cout << "Thread " << omp_get_thread_num() 
                << " processed index " << i << endl;
        }
    }

    // Print the updated array
    cout << "Updated Array: ";
    for (int i = 0; i < size; i++) {
        cout << array[i] << " ";
    }
    cout << endl;
}

void critical_section() {
    int sum = 0; // Shared variable

    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();

        // Each thread computes a local value
        int local_sum = thread_id * 10; // Example computation

        // Critical section to safely update the shared variable
        #pragma omp critical
        {
            sum += local_sum;
            cout << "Thread " << thread_id 
                 << " added " << local_sum 
                 << " to sum. Total sum is now " << sum << endl;
        }
    }

    // Final result
    cout << "Final sum: " << sum << endl;
}

void shared_private_reduction_variables() {
    const int size = 10;
    int shared_array[size];  // Shared array
    int private_var;         // Will be private to threads
    int sum = 0;             // Reduction variable

    // Initialize shared array
    for (int i = 0; i < size; i++) {
        shared_array[i] = i + 1; // [1, 2, 3, ..., 10]
    }

    #pragma omp parallel shared(shared_array, sum) private(private_var)
    {
        // Each thread gets a private copy of private_var
        private_var = omp_get_thread_num();

        #pragma omp for reduction(+:sum)
        for (int i = 0; i < size; i++) {
            cout << "Thread " << omp_get_thread_num()
                 << " processes index " << i 
                 << " with value " << shared_array[i] << endl;

            // Accumulate sum using reduction
            sum += shared_array[i];
            cout << "Sum " << sum << endl;
        }

        // Show the private variable for each thread
        cout << "Thread " << omp_get_thread_num() 
             << " has private_var = " << private_var << endl;
    }

    // Final output
    cout << "Final sum: " << sum << endl;
}

int main() {
    shared_private_reduction_variables();

    // critical_section();

    // parallel_for_loop();

    // nested_parallel_example2();
    // nested_parallel_example();

    // float array[100] = {0};
    // for(int i=0; i<100; i++) {
    //     cout << array[i] << " ";
    // }
    // cout << endl;
    // sub(array, 100);
    // for(int i=0; i<100; i++) {
    //     cout << array[i] << " ";
    // }
    // cout << endl;

    // int n=5;
    // float arr1[n] = {10, 20, 30, 40, 50};
    // float arr2[n] = {0, 0, 0, 0, 0};

    // for(int i=0; i<n; i++) {
    //     cout << arr1[i] << " ";
    // }
    // cout << endl;

    // simple(n, arr1, arr2);

    // for(int i=0; i<n; i++) {
    //     cout << arr2[i] << " ";
    // }
    // cout << endl;
    
    return 0;
}