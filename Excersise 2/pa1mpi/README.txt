Last name of Student 1:Senthil
First name of Student 1: Anishkumar
Email of Student 1:asenthil@ucsb.edu
Last name of Student 2:
First name of Student 2:
Email of Student 2:



Report for Question 2.a

For the normal matrix test case, performance was measured using 1, 2, and 4 cores, where each core runs one MPI process. 
The serial execution using a single core completed in approximately 16.78 seconds. When 
the computation was executed using 2 cores, the parallel runtime decreased to about 10.67 seconds, and when using 4 cores the 
runtime further decreased to roughly 6.06 seconds. GFLOPS performance increased from approximately 2.05 GFLOPS on 1 core to 
3.22 GFLOPS on 2 cores and 5.67 GFLOPS on 4 cores, demonstrating strong scaling as additional MPI processes were used.T
his shows a clear reduction in total execution time as more cores are used. Using these runtimes, the speedup for 2 cores 
is approximately 1.57, while the speedup for 4 cores is about 2.77. Although the speedup does not scale linearly with the number of cores, 
the improvement is still significant andindicates effective parallelization. The parallel efficiency for 2 cores is approximately 79 percent, 
and for 4 cores it isaround 69 percent. The decrease in efficiency as more cores are added is expected and is primarily due to 
communication overhead and synchronization costs between MPI processes

Report for Question 2.b

For the upper triangular matrix test case performance was measured using 1, 2, and 4 cores with 
one MPI process per core. The serial runtime was approximately 8.92 seconds. When using 2 cores, the runtime decreased slightly to about 
8.21 seconds, and when using 4 cores the runtime further decreased to approximately 6.33 seconds. GFLOP performance increased from approximately 1.93 
GFLOPS on 1 core to 2.09 GFLOPS on 2 cores and 2.71 GFLOPS on 4 cores, showing more limited scaling due to uneven work distribution across processes.
While increasing the number of cores does reduce the runtime, the improvement is significantly smaller than what was observed for the normal matrix case
in Part 2a. The reduced speedup and efficiency for the upper triangular matrix arise from load imbalance across MPI processes. Because many rows in 
an upper triangular matrix contain fewer nonzero elements, processes assigned to later rows perform substantially less computation and finish 
earlier, remaining idle while other processes continue working. As a result, communication and synchronization overhead dominate a larger 
fraction of the total runtime, limiting scalability as more cores are added.One way to address this issue is to use a round robin 
work distribution scheme. Instead of assigning contiguous blocks of rows to each MPI process, rows can be distributed cyclically across 
processes so that each process receives a mix of rows with both high and low computational cost. This approach helps balance the workload 
more evenly, reduces idle time, and improves overall parallel efficiency. By adopting round robin scheduling, the performance gap between 
the normal and upper triangular matrix cases can be reduced, leading to better utilization of available cores.