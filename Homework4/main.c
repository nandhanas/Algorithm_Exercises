#include<stdio.h>
#include <stdlib.h>
#include <time.h>
#include"binaryheap.h"

#define REPEAT 5
#define MAX_ELEM_VALUE 25

double get_execution_time(const struct timespec b_time,
                          const struct timespec e_time)
{
  return (e_time.tv_sec-b_time.tv_sec) +
                   (e_time.tv_nsec-b_time.tv_nsec)/1E9;
}

int main() {
    int n = 10;
    int a[MAX_SIZE],i,j;
    struct timespec b_time, e_time;
    for(i=0;i<REPEAT;i++)
    {
      srand(10);
      for(j=0;j<n;j++)
      {
        a[j] = rand()%(2*MAX_ELEM_VALUE);
      }
      build_max_heap(a, n);
      printf(" %d Binaryheap Execution Time = %lf sec\n",i, get_execution_time(b_time, e_time));
      insert(a, 55, &n);
      //printf("Min heap\n");
      //print_heap(a, n);
      extract_max(a, &n);
      //printf("Min heap\n");
      //print_heap(a, n);
    }
    return 0;
}
