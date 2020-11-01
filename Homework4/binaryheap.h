#include<stdio.h>
#define MAX_SIZE 15

int parent(int i);
int left_child(int i);
int right_child(int i);
void swap(int *x, int *y);
void insert(int a[], int data, int *n);
void max_heapify(int a[], int i, int n);
void build_max_heap(int a[], int n);
int get_max(int a[]);
int extract_max(int a[], int *n);
void print_heap(int a[], int n);
