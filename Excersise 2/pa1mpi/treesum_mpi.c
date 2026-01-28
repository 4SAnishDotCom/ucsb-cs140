/*
 * File:     treesum_mpi.c
 *
 * Purpose:  Use tree-structured communication to find the global sum
 *           of a random collection of ints.  This version doesn't
 *           require that comm_sz be a power of 2.
 *
 */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

/*-------------------------------------------------------------------
 * Function:
 *  global_sum
 *
 * Purpose:
 *  Implement a global sum using tree structured communication
 *
 * Notes:
 *  1.  The return value for global sum is only valid on process 0
 */
int global_sum(int my_int /* in */, int my_rank /* in */, int comm_sz /* in */,
               MPI_Comm comm /* in */) {
  int my_sum = my_int;
  int partner;


  for(int step = 1; step < comm_sz; step *=2 ){
      if(my_rank % (step*2) == 0){
      partner  =  my_rank + step;
      
      if(partner <comm_sz){
        int number;
        MPI_Recv(&number,1, MPI_INT, partner, 0, comm, MPI_STATUS_IGNORE);
        my_sum += number;
        }
    }
    else if(my_rank % step == 0){
      partner  = my_rank - step;
      MPI_Send(&my_sum, 1, MPI_INT, partner, 0, comm);
      break;
    }
  }
  return my_sum;
 /* Global_sum */
               }