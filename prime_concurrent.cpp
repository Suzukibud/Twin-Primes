/** 
    @file main.cpp 
    @author Damien Doran
    @date 16/02/21
    @copyright This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License. To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/4.0/.
*/

#include <iostream>
#include <chrono>
#include <omp.h>
#include <math.h>
using namespace std;
using namespace std::chrono;

/**
  * @brief concurrently calculates the number of twin primes
  * 
  * @param n is the number to calculate the primes up to
  * @return long long value count of the number of twins
  */
long concurrent(long long n)
{
    bool *a = new bool[n];
    int count = 0;

    omp_set_num_threads(32); 
    
    const long long lastNumSquared = sqrt(n);

    #pragma omp parallel for  // divide loop iterations for each thread
    // assigning true to all values
    for(long long i = 0L; i < n; i ++){ // L is saying this is a long value
        a[i] = true;
    }
    // mark false values
    #pragma omp parallel for schedule(dynamic)
    for(long long i = 2; i <= lastNumSquared; i++){
        if(a[i]){// if true check
            for(long long j =i*2; j <= n; j += i){ // start at first multiple of i
                a[j] = false;
            }
        } 
    }
    #pragma omp parallel for  // divide loop iterations for each thread
    for(long long i = 3; i < n-2; i += 2){ 
        if(a[i] && a[i+2])
        {
            #pragma omp critical  // open mp method to access critical areas, as there was a race condition happening
            count ++; 
        } 
    }
    delete[] a;
    return count;
}

int main(){
    long long n = 1000;
    
    cout << "The number input was : " << n << endl << endl;
    cout << "The number of threads used : " << omp_get_max_threads << "\n\n";

    auto time2 = high_resolution_clock::now();
    concurrent(n);
    auto stop_time2 = high_resolution_clock::now();
    auto duration2 = duration_cast<milliseconds>(stop_time2 - time2);

    cout << "Concurrent code " << endl;
    cout << "The number of twins : " << concurrent(n) << endl;
    cout << "The time taken : " << duration2.count() << endl;

    return 0;
}