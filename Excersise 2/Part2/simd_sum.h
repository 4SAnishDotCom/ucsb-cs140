/*
 * File: simd_sum.h
 *
 */

#define ALIGNMENT 32 

/*-------------------------------------------------------------------
 *  Sum n integers 
 */
extern int  sum_naive(int n, int *a); 


/*-------------------------------------------------------------------
 * Sum n integers  with 128-bit SSE vectorization after unrolling by 4
 */
extern int  sum_sse(int n, int *a);
 

/*-------------------------------------------------------------------
 *  AVX-256 vectorized summation of an array of 32-bit integers.
 */ 

extern int  sum_avx(int n, int *arr); 


