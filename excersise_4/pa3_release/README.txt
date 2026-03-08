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

To parallelize the Jacobi iteration on the GPU, I implemented a CUDA kernel
where each thread computes the update of one row of the matrix-vector
multiplication. A block mapping strategy was used where the grid consists of
b blocks and each block contains m threads, giving b×m total threads.
Each thread computes:

    y_i = d_i + A_i * x

where A_i is the i-th row of matrix A. The computation of the dot product
between row A_i and vector x is performed inside the kernel. After computing
the new value y_i, each thread stores the difference |y_i − x_i| into the
diff vector for convergence checking.

The host function launches the CUDA kernel with different block and thread
configurations, and iteratively updates x ← y until the iteration limit t is
reached or the convergence threshold is satisfied.

Parallel time for n=4K, t=1K, 4x128 threads

Time cost in seconds: 4.100723

Parallel time for n=4K, t=1K, 8x128 threads

Time cost in seconds: 2.052177

Parallel time for n=4K, t=1K, 16x128 threads

Time cost in seconds: 1.034997

Parallel time for n=4K, t=1K, 32x128 threads

Time cost in seconds: 0.560934


Do you see a trend of  speedup improvement  with more threads? We expect a good speedup and explain the reason.

Yes, the execution time decreases as the number of threads increases. When
using 4×128 threads the runtime is about 4.10 seconds, while with 32×128
threads the runtime reduces to about 0.56 seconds. This demonstrates a clear
speedup as more threads are used.

The improvement occurs because the matrix-vector multiplication is highly
parallelizable. Each row of the matrix can be processed independently, so
increasing the number of threads allows more rows to be computed
simultaneously. The GPU architecture is designed for massive parallelism,
allowing thousands of threads to execute concurrently. With more blocks and
threads, the workload is better distributed across the streaming
multiprocessors, increasing hardware utilization and reducing total runtime.

----------------------------------------------------------------------------


Report for Question 2 
List your code change for this question

For this question I implemented the asynchronous Gauss-Seidel update on the
GPU. Instead of synchronizing after every iteration, each thread repeatedly
updates its assigned element of vector y for a batch of r iterations before
performing the convergence check.

Specifically, each thread performs the following update repeatedly:

    y_i = d_i + A_i * y

for r iterations without synchronization. After completing the batch,
the error value e_i = |y_i − x_i| is computed and stored in the diff array.
The host then computes the global error and checks the convergence condition.
The iteration counter is increased by r after each batch.

This asynchronous update reduces synchronization overhead and allows threads
to proceed independently, improving performance in many cases.

Let the default number of asynchronous iterations be 5 in a batch as specified in it_mult_vec.h.
List reported parallel time and the number of actual iterations executed  for n=4K, t=1K, 8x128  threads with asynchronous Gauss Seidel

Time cost in seconds: 0.038385  
Number of iterations executed: 15

List reported parallel time and the number of actual iterations executed  for n=4K, t=1K,  32x128 threads with asynchronous Gauss Seidel

Time cost in seconds: 0.440201  
Number of iterations executed: 1025


Is the number of iterations  executed by  above parallel asynchronous Gauss Seidel-Seidel method  bigger or smaller  than that
of the sequential Gauss Seidel-Seidel code under the same converging error threshold (1e-3)?  
Explain the reason based on the running trace of above two thread configurations that more threads may not yield more time reduction in this case. 

The number of iterations executed by the asynchronous parallel
Gauss-Seidel method can be larger than the sequential Gauss-Seidel method.
This happens because threads update elements of the vector independently
without synchronization, so some threads may read stale values of other
elements during the update. As a result, the convergence process may require
more iterations.

From the running trace, the configuration with 8×128 threads converged after
only 15 iterations due to early convergence detection, resulting in a very
small runtime of about 0.038 seconds. However, the configuration with
32×128 threads executed the full iteration count (about 1025 iterations)
and took about 0.44 seconds.

This demonstrates that increasing the number of threads does not always lead
to faster convergence. With more threads updating values asynchronously,
the updates can become less consistent, which may slow down convergence and
increase the number of iterations required. Therefore, more threads may not
always produce better performance for asynchronous iterative methods.

Make sure you attach the  output trace  of your code below in running the tests of the unmodified it_mult_vec_test.cu on Expanse GPU for Q1 and Q2

Test 1:n=4, t=1, 1x2 threads:
With totally 1*2 threads, matrix size being 4, t being 1
Time cost in seconds: 0.111581
Final error (|y-x|): 1.750000.
# of iterations executed: 1.
Final y[0]=1.750000. y[n-1]=1.750000

Test 2:n=4, t=2, 1x2 threads:
With totally 1*2 threads, matrix size being 4, t being 2
Time cost in seconds: 0.000266
Final error (|y-x|): 1.312500.
# of iterations executed: 2.
Final y[0]=0.437500. y[n-1]=0.437500

Test 3:n=8, t=1, 1x2 threads:
With totally 1*2 threads, matrix size being 8, t being 1
Time cost in seconds: 0.000246
Final error (|y-x|): 1.875000.
# of iterations executed: 1.
Final y[0]=1.875000. y[n-1]=1.875000

Test 4:n=8, t=2, 1x2 threads:
With totally 1*2 threads, matrix size being 8, t being 2
Time cost in seconds: 0.000259
Final error (|y-x|): 1.640625.
# of iterations executed: 2.
Final y[0]=0.234375. y[n-1]=0.234375

Test 8a:n=4, t=1, 1x1 threads/Gauss-Seidel:
With totally 1*1 threads, matrix size being 4, t being 1
Time cost in seconds: 0.000250
Final error (|y-x|): 1.000193.
# of iterations executed: 5.
Final y[0]=1.000089. y[n-1]=1.000193

Test 8b:n=4, t=2, 1x1 threads/Gauss-Seidel:
With totally 1*1 threads, matrix size being 4, t being 2
Time cost in seconds: 0.000236
Final error (|y-x|): 1.000193.
# of iterations executed: 5.
Final y[0]=1.000089. y[n-1]=1.000193

Test 8c:n=8, t=1, 1x1 threads/Gauss-Seidel:
With totally 1*1 threads, matrix size being 8, t being 1
Time cost in seconds: 0.000241
Final error (|y-x|): 1.001155.
# of iterations executed: 5.
Final y[0]=1.001155. y[n-1]=0.999790

Test 8d:n=8, t=2, 1x1 threads/Gauss-Seidel:
With totally 1*1 threads, matrix size being 8, t being 2
Time cost in seconds: 0.000241
Final error (|y-x|): 1.001155.
# of iterations executed: 5.
Final y[0]=1.001155. y[n-1]=0.999790

Test 9: n=4K t=1K 32x128 threads:
With totally 32*128 threads, matrix size being 4096, t being 1024
Time cost in seconds: 0.560934
Final error (|y-x|): 1.557740.
# of iterations executed: 1024.
Final y[0]=0.221225. y[n-1]=0.221225

Test 9a: n=4K t=1K 16x128 threads:
With totally 16*128 threads, matrix size being 4096, t being 1024
Time cost in seconds: 1.034997
Final error (|y-x|): 1.557740.
# of iterations executed: 1024.
Final y[0]=0.221225. y[n-1]=0.221225

Test 9b: n=4K t=1K 8x128 threads:
With totally 8*128 threads, matrix size being 4096, t being 1024
Time cost in seconds: 2.052177
Final error (|y-x|): 1.557740.
# of iterations executed: 1024.
Final y[0]=0.221225. y[n-1]=0.221225

Test 9c: n=4K t=1K 4x128 threads:
With totally 4*128 threads, matrix size being 4096, t being 1024
Time cost in seconds: 4.100723
Final error (|y-x|): 1.557740.
# of iterations executed: 1024.
Final y[0]=0.221225. y[n-1]=0.221225

Test 11: n=4K t=1K 32x128 threads/Async:
With totally 32*128 threads, matrix size being 4096, t being 1024
Time cost in seconds: 0.440201
Final error (|y-x|): 0.001953.
# of iterations executed: 1025.
Final y[0]=1.000915. y[n-1]=1.000976

Test 11a: n=4K t=1K 8x128 threads/Async:
With totally 8*128 threads, matrix size being 4096, t being 1024
Time cost in seconds: 0.038385
Final error (|y-x|): 0.000000.
# of iterations executed: 15.
Early exit due to convergence, even asked for 1024 iterations.
Asynchronous code actually runs 15 iterations.
Final y[0]=1.000000. y[n-1]=1.000000

Summary: Failed 0 out of 14 tests

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

The highest GFLOPS observed on the NVIDIA V100 GPU is 7439.968 GFLOPS using cuBLAS DGEMM. From PA2, the highest GFLOPS 
observed using the MKL GEMM implementation on the CPU host when N=1600 is 146.10 GFLOPS. The speedup of the V100 GPU over the CPU host is therefore:
Speedup = 7439.968 / 146.10 = 50.92×
This shows that the GPU provides significantly higher computational throughput for matrix-matrix multiplication due to its massive parallelism and highly optimized cuBLAS implementation compared with the CPU-based MKL implementation.