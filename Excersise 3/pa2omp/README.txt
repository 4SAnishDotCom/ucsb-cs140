Last name of Student 1:Senthil
First name of Student 1:Anishkumar
Email of Student 1:asenthil@ucsb.edu



If CSIL is used for performance assessment instead of Expanse, make sure you evaluate when such a machine is lightly 
loaded using “uptime”. Please  indicate your evaluation is done on CSIL and list the uptime index of that CSIL machine.  

Report 
----------------------------------------------------------------------------
1. How is the code parallelized? Show your solution by listing the key computation parallelized with
  OpenMP and related code. 

  The code is parallelized using OpenMP by distributing the outer loop that computes each row of the matrix vector
   multiplication across multiple threads. Each iteration computes one row of the result vector, and since each row is 
   independent, they can execute concurrently without data races. The parallelization is implemented using a pragma omp 
   parallel for directive with different scheduling policies depending on the mapping type. Static scheduling corresponds 
   to block mapping. Static scheduling with a chunk size corresponds to block cyclic mapping. Dynamic scheduling with a 
   chunk size corresponds to dynamic mapping. After computing the new vector y, a second parallel loop copies vector y into
  vector x to complete each iteration of the algorithm.
  parallelized portion of the code is attatched below: 

 omp_set_num_threads(threadcnt);
  for (k = 0; k < no_iterations; k++) {
    if(mappingtype == BLOCK_MAPPING){
      #pragma omp parallel for schedule(static)
      for (i = 0; i < matrix_dim; i++) {
      mv_compute(i);
    }
    }
    else if(mappingtype == BLOCK_CYCLIC){
      #pragma omp parallel for schedule(static, chunksize)
      for (i = 0; i < matrix_dim; i++) {
      mv_compute(i);
    }
    }
    else if(mappingtype == BLOCK_DYNAMIC){
      #pragma omp parallel for schedule(dynamic, chunksize)
      for (i = 0; i < matrix_dim; i++) {
      mv_compute(i);
    }
    }

    #pragma omp parallel for schedule(static)
    for (i = 0; i < matrix_dim; i++) {
      vector_x[i] = vector_y[i];}
  }

----------------------------------------------------------------------------
2.  Report the parallel time, speedup, and efficiency with blocking mapping, block cyclic mapping with block size 1 
and block size 16 using  2 cores (2 threads), and 4 cores (4 threads) for parallelizing the code 
in handling a full dense matrix with n=4096 and t=1024. 
For the full dense matrix with n equals 4096 and t equals 1024, the one thread baseline time for block mapping was 19.706787 
seconds. With two threads, block mapping reduced the time to 9.996690 seconds, giving a speedup of 1.97 and an efficiency of
 0.99. With four threads, block mapping reduced the time to 5.683351 seconds, giving a speedup of 3.47 and an efficiency of 
 0.87. For block cyclic with block size 1, the one thread time was 19.340274 seconds, the two thread time was 13.781769 seconds
  with speedup 1.40 and efficiency 0.70, and the four thread time was 10.162973 seconds with speedup 1.90 and efficiency 0.48. 
  For block cyclic with block size 16, the one thread time was 19.318601 seconds, the two thread time was 10.842215 seconds with 
  speedup 1.78 and efficiency 0.89, and the four thread time was 5.988470 seconds with speedup 3.23 and efficiency 0.81. 
  Block mapping performed best overall for the dense matrix because the workload per row is uniform, so contiguous static 
  scheduling minimizes overhead and preserves good cache locality, while cyclic scheduling introduces more scheduling overhead 
  and can reduce locality.

----------------------------------------------------------------------------
3.  Report the parallel time, speedup, and efficiency with blocking mapping, block cyclic mapping with block size 1 
and block size 16 using  2 cores (2 threads), and 4 cores (4 threads) for parallelizing the code 
in handling an upper triangular matrix (n=4096 and t=1024).

Write a short explanation on why one mapping method is significantly faster than or similar to another.

For the upper triangular matrix with n equals 4096 and t equals 1024, the one thread baseline time for block 
mapping was 10.528311 seconds. With two threads, block mapping reduced the time to 7.677676 seconds, giving a 
speedup of 1.37 and an efficiency of 0.68. With four threads, block mapping reduced the time to 4.599120 seconds, giving 
a speedup of 2.29 and an efficiency of 0.57. For block cyclic with block size 1, the one thread time was 10.541124 seconds
, the two thread time was 5.917319 seconds with speedup 1.78 and efficiency 0.89, and the four thread time was 4.632964
 seconds with speedup 2.28 and efficiency 0.57. For block cyclic with block size 16, the one thread time was 10.509612 seconds, 
the two thread time was 5.747719 seconds with speedup 1.83 and efficiency 0.91, and the four thread time was 2.811711 seconds
with speedup 3.74 and efficiency 0.94. For dynamic scheduling with block size 16, the one thread time was 10.778039 seconds,
the two thread time was 5.946578 seconds with speedup 1.81 and efficiency 0.90, and the four thread time was 3.369642 seconds
with speedup 3.20 and efficiency 0.80. For the upper triangular case, block cyclic with block size 16 performed best at four
threads because the workload per row decreases as the row index increases, creating load imbalance under simple block 
 mapping. Cyclic distribution spreads heavy and light rows more evenly across threads, reducing imbalance and improving parallel efficiency.
