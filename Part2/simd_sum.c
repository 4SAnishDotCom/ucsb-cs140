/*
 * File: simd_sum.c
 *
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <immintrin.h> // Header for AVX intrinsics

// include SSE intrinsics
#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#include <x86intrin.h>
#endif


#include "simd_sum.h"

/*-------------------------------------------------------------------
 *  Sum n integers 
 */
int  sum_naive(int n, int *a) {
    int  sum = 0;
	  for (int i = 0; i < n; i++) {
		    sum += a[i];
	  }
    return sum;
}


/*-------------------------------------------------------------------
 * Sum n integers  with 128-bit SSE vectorization after unrolling by 4
 */
int  sum_sse(int n, int *a) {
    __m128i sum4=_mm_setzero_si128();
    int  result=0;
    for (int i=0;i<n/4*4;i=i+4) {
      __m128i temp=_mm_loadu_si128((__m128i *)(a+i));
      sum4=_mm_add_epi32(sum4,temp);
    }
    int s4[4] __attribute__((aligned(16)));//this array is started at a memory address that is a multiple of 16 bytes
    _mm_storeu_si128((__m128i *)s4,sum4);
    result =s4[0]+s4[1]+s4[2]+s4[3];
    for (int i=n/4*4;i<n;i++) {
    	result+= a[i];
    }  
    return result;
}

/*-------------------------------------------------------------------
 * @brief AVX-256 vectorized summation of an array of 32-bit integers.
 * This function uses __m256i registers and integer intrinsics.
 * @param arr Pointer to the array of integers.
 * @param n Number of elements to sum.
 * @return The vectorized sum.
 */ 

int  sum_avx(int n, int *arr) {
    // __m256i is the data type for 256-bit AVX integer registers (8 x 32-bit integers)
    __m256i vec_sum = _mm256_setzero_si256(); // Initialize vector accumulator to 0
    int  final_sum = 0;
    // Your solution    

    // Loop through the array in chunks of 8 elements
        // Load 8 UNALIGNED 32-bit integers from memory into a register
        // Vector addition: 8 parallel 32-bit integer additions

    // --- Horizontal Summation (Reducing the 8 elements to a single scalar) ---
    // 1. Store the vector sum back to a temporary 32-byte aligned array of 8 integers. 
    // This array can be aligned with 32 bytes for 256 bits

    // 2. Scalar sum of the 8 temporary elements 

    // --- Handle the remainder elements (if N is not a multiple of 8) ---


    return final_sum;
}


