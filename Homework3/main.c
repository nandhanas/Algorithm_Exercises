#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sorting.h"

#define MAX_ELEM_VALUE 25
#define REPEAT 5

double get_execution_time(const struct timespec b_time,
                          const struct timespec e_time)
{
  return (e_time.tv_sec-b_time.tv_sec) +
                   (e_time.tv_nsec-b_time.tv_nsec)/1E9;
}

int partition_cases (int arr[], int low, int high, int pivot)
{
    //int pivot = arr[high];    // pivot
    int i = (low - 1);  // Index of smaller element

    for (int j = low; j <= high- 1; j++)
    {
        // If current element is smaller than the pivot
        if (arr[j] < pivot)
        {
            i++;    // increment index of smaller element
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort_bestcase(int arr[], int low, int high)
{
    if (low < high)
    {
        int pivot = arr[low];
        int pi = partition_cases(arr, low, high, pivot);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void quickSort_worstcase(int arr[], int low, int high)
{
    if (low < high)
    {
        int mid = (low+high)/2;
        int pivot = arr[mid];
        int pi = partition_cases(arr, low, high, pivot);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

int main()
{
    int A[10],B[10],C[10],D[10],E[10],F[10],G[10],i,j;
    int n = sizeof(A)/sizeof(A[0]);
    struct timespec b_time, e_time;

    for(i=0;i<REPEAT;i++)
    {
      srand(10);
      for(j=0;j<n;j++)
      {
        A[j] = rand()%(2*MAX_ELEM_VALUE);
        B[j] = rand()%(2*MAX_ELEM_VALUE);
        C[j] = rand()%(2*MAX_ELEM_VALUE);
        D[j] = rand()%(2*MAX_ELEM_VALUE);
        E[j] = rand()%(2*MAX_ELEM_VALUE);
        F[j] = rand()%(2*MAX_ELEM_VALUE);
        G[j] = rand()%(2*MAX_ELEM_VALUE);
      }
      //Insertion sort
      clock_gettime(CLOCK_REALTIME, &b_time);
      insertionSort(A, n);
      clock_gettime(CLOCK_REALTIME, &e_time);
      //printf("Insertion sorted array: \n");
      //printArray(A, n);
      printf("%d Insertion sort Execution Time = %lf sec\n",i, get_execution_time(b_time, e_time));

      //Quick sort
      clock_gettime(CLOCK_REALTIME, &b_time);
      quickSort(B, 0, n-1);
      clock_gettime(CLOCK_REALTIME, &e_time);
      //printf("Quick sorted array: \n");
      //printArray(B, n);
      printf("%d Quicksort Execution Time = %lf sec\n",i, get_execution_time(b_time, e_time));

      //Heap sort
      clock_gettime(CLOCK_REALTIME, &b_time);
      heapSort(C, n);
      clock_gettime(CLOCK_REALTIME, &e_time);
      //printf("Heap sorted array: \n");
      //printArray(C, n);
      printf("%d Heapsort Execution Time = %lf sec\n",i, get_execution_time(b_time, e_time));

      //Selection sort
      clock_gettime(CLOCK_REALTIME, &b_time);
      selectionSort(D, n);
      clock_gettime(CLOCK_REALTIME, &e_time);
      //printf("Selection Sorted array: \n");
      //printArray(D, n);
      printf("%d Selectionsort Execution Time = %lf sec\n",i, get_execution_time(b_time, e_time));

      //Counting sort
      clock_gettime(CLOCK_REALTIME, &b_time);
      countingSort(E, n);
      clock_gettime(CLOCK_REALTIME, &e_time);
      //printf("Counting Sorted array: \n");
      //printArray(E, n);
      printf("%d Countingsort Execution Time = %lf sec\n",i, get_execution_time(b_time, e_time));

      //Radix sort
      clock_gettime(CLOCK_REALTIME, &b_time);
      radixSort(F, n);
      clock_gettime(CLOCK_REALTIME, &e_time);
      //printf("Radix Sorted array: \n");
      //printArray(F, n);
      printf("%d Radixsort Execution Time = %lf sec\n",i, get_execution_time(b_time, e_time));

      //bucket sort
      clock_gettime(CLOCK_REALTIME, &b_time);
      radixSort(G, n);
      clock_gettime(CLOCK_REALTIME, &e_time);
      //printf("Bucket Sorted array: \n");
      //printArray(G, n);
      printf("%d Bucketsort Execution Time = %lf sec\n",i, get_execution_time(b_time, e_time));
      printf("----------------------------------------------------\n");
    }

    size_t m = 200;
    int arr[m], worst[m];
    for(j=0;j<m;j++)
    {
      arr[j] = rand()%(2*MAX_ELEM_VALUE);
    }

    //Insertion sort Bestcase
    insertionSort(arr,n);
    //printArray(arr,n);
    clock_gettime(CLOCK_REALTIME, &b_time);
    insertionSort(arr, n);
    clock_gettime(CLOCK_REALTIME, &e_time);
    printf("Insertion sort Bestcase Execution Time(O(n)) = %lf sec\n", get_execution_time(b_time, e_time));

    //Quick sort Bestcase
    clock_gettime(CLOCK_REALTIME, &b_time);
    quickSort_bestcase(arr,0,n-1);
    clock_gettime(CLOCK_REALTIME, &e_time);
    printf("Quick sort Bestcase Execution Time(O(nlogn))= %lf sec\n", get_execution_time(b_time, e_time));

    //Insertion sort Worstcase
    revereseArray(arr,0,n-1);
    //printArray(arr,n);
    clock_gettime(CLOCK_REALTIME, &b_time);
    insertionSort(arr, n);
    clock_gettime(CLOCK_REALTIME, &e_time);
    printf("Insertion sort Worstcase Execution Time(O(n^2)) = %lf sec\n", get_execution_time(b_time, e_time));

    //Quick sort Worstcase
    clock_gettime(CLOCK_REALTIME, &b_time);
    quickSort_worstcase(arr,0,n-1);
    clock_gettime(CLOCK_REALTIME, &e_time);
    printf("Quick sort Worstcase Execution Time(O(n^2)) = %lf sec\n", get_execution_time(b_time, e_time));

    int repeat[10] = {1,2,3,2,4,5,3,4,4,5};
    //Selection sort of repeated array
    clock_gettime(CLOCK_REALTIME, &b_time);
    selectionSort(repeat, n);
    clock_gettime(CLOCK_REALTIME, &e_time);
    printf("Selection Sorted array: \n");
    printArray(repeat, n);
    printf("Selectionsort Execution Time = %lf sec\n", get_execution_time(b_time, e_time));



    return 0;
}
