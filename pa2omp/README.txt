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


----------------------------------------------------------------------------
3.  Report the parallel time, speedup, and efficiency with blocking mapping, block cyclic mapping with block size 1 
and block size 16 using  2 cores (2 threads), and 4 cores (4 threads) for parallelizing the code 
in handling an upper triangular matrix (n=4096 and t=1024).

Write a short explanation on why one mapping method is significantly faster than or similar to another.
