/*
 * File: simd_test.c
 *
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "minunit.h"
#include "simd_sum.h"

#define TRUE 1
#define FALSE 0
#define REPEAT 3
#define VEC_SIZE 8

#define N 10000000
// ALIGNMENT=32
// Create a 32-byte-aligned array  allocated at a memory address that is a multiple of 32 bytes
int testarray[N] __attribute__((aligned(ALIGNMENT)));

/*-------------------------------------------------------------------
 *  Generate   data with random values 
 */
void gendata() {
	  for (int i = 0; i < N; i++) {
		  testarray[i] = i%10;
  	}
}


/*-------------------------------------------------------------------
 * Test summation and report time and GFLOP/s
 * If failed, return a message string showing the failed point.
 * If successful, return NULL.
 */

char *benchmark(int n, int *a, int (*computeSum)(int, int*), char *name, int unittest, int  expected){
    int i; 
    int  sum=0;
    sum = computeSum(n, a); //warmup

    double start=get_time()*1000000;
    for (i=0;i<REPEAT; i++)
      sum = computeSum(n, a);
    double diff = (get_time()*1000000-start)/REPEAT; //time in microseconds
    double mflops = ((double)n)/diff; //megaflops
 
    // print performance results
	  printf("%20s: ", name);
		printf("Time:  %.4f microseconds. GFLOPS:  %.2f\n", diff, mflops/1000.0);
    if(unittest) {
        mu_assert("Sum result does not match", sum == expected);
    }
    return NULL; 
}

/*-------------------------------------------------------------------
 * Run one test for the implemented solution  
 */
void test1a(){
    char *error;
    error=benchmark(N, testarray, sum_naive, "Naive sum", FALSE, 0);
}

void test2a(){
    char *error;
    error=benchmark(N, testarray, sum_sse, "SSE-128", FALSE, 0);
}

char *test3(){
    char *error;
    int  expected = sum_naive(N, testarray);
    error=benchmark(N, testarray, sum_avx, "AVX-256", TRUE, expected);
    if(error !=NULL)
        printf("Error sum-avx-256: %s\n", error);
    return error;
}
/*-------------------------------------------------------------------
 * Run all tests.  Ignore returned messages if we donot care
 */
void run_all_tests(void) {
    gendata(); 
    test1a();
    test2a();
    mu_run_test(test3);
}

/*-------------------------------------------------------------------
 * The main entrance to run all tests.
 * Print the test stats.
 */
int main(int argc, char *argv[]) {
     run_all_tests();
     mu_print_test_summary("Summary:");
     return 0;
}





