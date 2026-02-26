Last name of Student 1:
First name of Student 1:
Email of Student 1:
Last name of Student 2:
First name of Student 2:
Email of Student 2:

See the description of this assignment  for detailed reporting requirements 


Part B

Q2.a List parallel code that uses at most two barrier calls inside the while loop

Inside the while loop each thread first computes its assigned rows of y using the current x values according to either block or block cyclic mapping.
After finishing its portion of y each thread calls pthread barrier wait to ensure all threads complete the matrix vector computation phase.
Each thread then computes a local convergence flag by checking whether the absolute difference between its rows of x and y is below the threshold.
A second pthread barrier wait is used so that all local convergence flags are available before determining a global convergence condition.
One designated thread combines the local flags into a single global stop value and if convergence is reached all threads exit the loop.

Block mapping:
  int blocksize = ceil((double)matrix_dim / thread_count);
  int row_start = blocksize*my_rank;
  int row_end = blocksize*(my_rank+1)-1;

  if (my_rank == thread_count-1) row_end = matrix_dim-1;
  if (my_rank == 0) global_stop = 1;

  for (int k = 0; k < no_iterations; k++) {
    pthread_barrier_wait(&mybarrier);

    for (int i = row_start; i <= row_end ; i++) {
      mv_compute(i);
      double err = fabs(vector_x[i]-vector_y[i]); 
      if (err > ERROR_THRESHOLD) global_stop = 0; 
    }

    pthread_barrier_wait(&mybarrier);

    if (global_stop) break; 

    for (int i = row_start; i <= row_end; i++) vector_x[i] = vector_y[i];
  } 

cyclic :

if (my_rank == 0) global_stop = 1;

  for (int k = 0; k < no_iterations; k++) {
    pthread_barrier_wait(&mybarrier);

    for (int i = 0; i < matrix_dim; i++) {
      if ((i / cyclic_blocksize) % thread_count == my_rank) {
        mv_compute(i);
        double err = fabs(vector_x[i]-vector_y[i]); 
        if (err > ERROR_THRESHOLD) global_stop = 0; 
      }
    }

    pthread_barrier_wait(&mybarrier);

    if (global_stop) break; 

    for (int i = 0; i < matrix_dim; i++) {
      if ((i / cyclic_blocksize) % thread_count == my_rank) vector_x[i] = vector_y[i];
    }
  } 



Q2.b Report parallel time, speedup, and efficiency for  the upper triangular test matrix case when n=4096 and t=1024. 
Use 2 threads and 4  threads (1 thread per core) under blocking mapping, and block cyclic mapping with block size 1 and block size 16.    
Write a short explanation on why one mapping method is significantly faster than or similar to another.

For n equals 4096 and t equals 1024 the sequential runtime with one thread was approximately 12.88 seconds for block mapping and similar for cyclic mapping.
With two threads block mapping achieved about 9.64 seconds giving a speedup of 1.34 and efficiency of roughly 0.67.
Block cyclic with block size one achieved 6.75 seconds giving a speedup of 1.91 and efficiency of 0.95.
Block cyclic with block size sixteen achieved 7.83 seconds giving a speedup of 1.64 and efficiency of 0.82.
With four threads block mapping achieved 6.28 seconds giving a speedup of 2.05 and efficiency of about 0.51.
Block cyclic with block size one achieved 4.67 seconds giving a speedup of 2.76 and efficiency of 0.69.
Block cyclic with block size sixteen achieved 4.48 seconds giving a speedup of 2.87 and efficiency of 0.72.

Block mapping performs worse because the upper triangular matrix creates uneven work across rows causing load imbalance when rows are assigned contiguously.
Earlier rows contain more nonzero entries while later rows contain fewer so some threads finish earlier and wait at barriers reducing parallel efficiency.
Block cyclic mapping distributes heavy and light rows more evenly across threads improving balance and reducing idle time during synchronization.
Smaller block sizes such as one improve balance further but may increase scheduling overhead while moderate sizes like sixteen balance locality and load effectively.









Please indicate if your evaluation is done on CSIL and if yes, list the uptime index of that CSIL machine.  


-----------------------------------------------------------------
Part C

1. Report what code changes you made for blasmm.c. 

To complete Part C, I implemented the missing BLAS2 matrix matrix multiplication using repeated calls to 
cblas_dgemv(). Since MKL uses column major storage by default, I computed each column of the output matrix C 
by multiplying matrix A with the corresponding column of matrix B. Inside the loop over columns j, I defined a 
pointer to the j-th column of B using &B[jK] and a pointer to the j-th column of the result matrix C using 
&C_dgemv[jM]. I then called cblas_dgemv with CblasColMajor and CblasNoTrans, passing M and K as the dimensions, 
alpha = 1.0 indicating no spaces in the A matrix, beta = 0.0 meaning that the values in the C matrix are overwritten with whateve we calculated
, and increments equal to 1. This computes each column of C as A times the corresponding column of B. No other
 structural changes were made to the file. 


2. Conduct a latency and GFLOPS comparison of the above 3 when matrix dimension N varies as 50, 200, 800, and 1600. 
Run the code in one thread and 8 threads on an AMD CPU server of Expanse.
List the latency and GFLOPs of  each method in each setting.  
Explain why when N varies from small to large,  Method 1 with GEMM starts to outperform others. 

The performance of the three implementations, MKL GEMM, repeated GEMV calls, and the naive triple loop, was measured 
for matrix sizes N = 50, 200, 800, and 1600 using both 1 thread and 8 threads on an AMD CPU server of Expanse. 
For small matrices such as N = 50, performance differences were small and parallel scaling was limited. In some cases, the 8-thread execution was slower due to thread creation and synchronization overhead dominating the short 
computation time.

As N increased to 200 and 800, GEMM began to clearly outperform both GEMV and the naive implementation in terms of GFLOPS. 
For N = 1600, the difference became significant. With 8 threads, GEMM achieved much higher GFLOPS than both GEMV and the naive implementation.
GEMV provided moderate improvement compared to the naive method but scaled poorly compared to GEMM. The naive triple-loop
implementation had the lowest overall performance and benefited least from multithreading.Explain why when N varies from small to 
large, Method 1 with GEMM starts to outperform others.

As the matrix size increases, GEMM outperforms both the GEMV-based method and the naive triple-loop implementation because of higher arithmetic 
intensity and better cache utilization. GEMM performs a large number of floating point operations relative to the 
amount of memory accessed. This allows it to reuse data in cache efficiently. The MKL implementation of GEMM uses 
blocking, vectorization, and optimized memory access patterns, which significantly improve performance and 
scalability on multiple cores. In contrast, the GEMV-based method is more memory-bound because it repeatedly performs matrix-vector operations. 
Its performance is limited by memory bandwidth rather than compute capability. The naive implementation lacks blocking and advanced optimizations, 
leading to poor cache locality and lower performance.
For small N, overhead from threading can dominate execution time, limiting parallel speedup. As N becomes large, computation dominates overhead, and 
GEMM is able to fully utilize multiple cores, resulting in significantly higher GFLOPS and better scaling behavior.