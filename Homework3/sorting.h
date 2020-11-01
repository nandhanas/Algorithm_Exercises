#include <math.h>
#include <stdio.h>

//Insertion sort
void insertionSort(int arr[], int n);
void printArray(int arr[], int n);
void revereseArray(int arr[], int start, int end);


//Quick sort
void swap(int* a, int* b);
int partition (int arr[], int low, int high);
void quickSort(int arr[], int low, int high) ;

//Heap sort
void heapify(int arr[], int n, int i);
void heapSort(int arr[], int n);

//Counting sort
void countingSort(int arr[], int size);

//Selection sort
void selectionSort(int arr[], int n);

//Radix sort
int getMax (int arr[], int n);
void radixSort (int arr[], int n);

//Bucket sort
int compareIntegers(const void* first, const void* second);
void bucketSort(int array[],int n);
