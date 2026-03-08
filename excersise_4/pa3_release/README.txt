Last name of Student 1:
First name of Student 1:
Email of Student 1:
GradeScope account name of Student 1: 
Last name of Student 2:
First name of Student 2:
Email of Student 2:
GradeScope account name of Student 2: 


----------------------------------------------------------------------------
Report for Question 1 

List your code change for this question 


Parallel time for n=4K, t=1K,  4x128  threads

Parallel time for n=4K, t=1K,  8x128  threads

Parallel time for n=4K, t=1K,  16x128 threads

Parallel time for n=4K, t=1K,  32x128 threads


Do you see a trend of  speedup improvement  with more threads? We expect a good speedup and explain the reason.


----------------------------------------------------------------------------


Report for Question 2 
List your code change for this question




Let the default number of asynchronous iterations be 5 in a batch as specified in it_mult_vec.h.
List reported parallel time and the number of actual iterations executed  for n=4K, t=1K, 8x128  threads with asynchronous Gauss Seidel


List reported parallel time and the number of actual iterations executed  for n=4K, t=1K,  32x128 threads with asynchronous Gauss Seidel


Is the number of iterations  executed by  above parallel asynchronous Gauss Seidel-Seidel method  bigger or smaller  than that
of the sequential Gauss Seidel-Seidel code under the same converging error threshold (1e-3)?  
Explain the reason based on the running trace of above two thread configurations that more threads may not yield more time reduction in this case. 



Make sure you attach the  output trace  of your code below in running the tests of the unmodified it_mult_vec_test.cu on Expanse GPU for Q1 and Q2

----------------------------------------------------------------------------

Report for Question 3

List your solution to call  cublasDgemm() in Method 1.

To implement Method 1, I used the cuBLAS library routine cublasDgemm() to perform matrix–matrix multiplication on the GPU. 
The following call was added inside the run_cublas_dgemm() function:

cublasDgemm(handle,
            CUBLAS_OP_N,
            CUBLAS_OP_N,
            size_N,
            size_N,
            size_N,
            &alpha,
            d_A,
            size_N,
            d_B,
            size_N,
            &beta,
            d_C,
            size_N);

This computes the matrix multiplication C = alpha * A * B + beta * C. In this program alpha = 1.0 and beta = 0.0, so the 
operation becomes C = A * B. The matrices are stored in column-major format, which matches the native storage format 
expected by cuBLAS, so no transposition is required and the leading dimensions lda, ldb, and ldc are set to size_N.


List the latency and GFLOPs of the above 3 version of implementation and the number of Cuda threads used in executing Method 3 
when matrix dimension N varies as 50, 200, 800,  and 1600.  

Matrix size N = 50

Method 1 (cuBLAS DGEMM)
Latency: 0.112 ms
GFLOPS: 2.240

Method 2 (cuBLAS DGEMV in a loop)
Latency: 0.158 ms
GFLOPS: 1.585

Method 3 (Naive GEMM kernel)
Latency: 0.009 ms
GFLOPS: 27.127


Matrix size N = 200

Method 1 (cuBLAS DGEMM)
Latency: 11.511 ms
GFLOPS: 1.390

Method 2 (cuBLAS DGEMV in a loop)
Latency: 3.231 ms
GFLOPS: 4.952

Method 3 (Naive GEMM kernel)
Latency: 0.029 ms
GFLOPS: 558.036


Matrix size N = 800

Method 1 (cuBLAS DGEMM)
Latency: 0.232 ms
GFLOPS: 4408.928

Method 2 (cuBLAS DGEMV in a loop)
Latency: 12.620 ms
GFLOPS: 81.142

Method 3 (Naive GEMM kernel)
Latency: 1.083 ms
GFLOPS: 945.180


Matrix size N = 1600

Method 1 (cuBLAS DGEMM)
Latency: 1.328 ms
GFLOPS: 6168.823

Method 2 (cuBLAS DGEMV in a loop)
Latency: 46.028 ms
GFLOPS: 177.979

Method 3 (Naive GEMM kernel)
Latency: 7.182 ms
GFLOPS: 1140.576


The CUDA kernel in Method 3 uses a block size of 20 × 20 threads. Therefore each block contains 400 CUDA threads. 
The grid size is determined as ceil(N/20) × ceil(N/20). For example when N = 1600, the grid becomes 80 × 80 blocks. 
This results in a total of 80 × 80 × 400 = 2,560,000 CUDA threads being launched for the computation.


List the highest gigaflops you have observed with V100 from this question and the highest gigaflops  you have observed from PA2 MKL GEMM code  when N=1600.  
Compute the ratio between these two numbers as the speedup of V100 over a CPU host. 

