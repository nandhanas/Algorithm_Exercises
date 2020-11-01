#include <stdio.h>
#define V 9
//utility functions
void randomly_fill_matrix(int A[V][V], const size_t A_rows, const size_t A_cols);
double get_execution_time(const struct timespec b_time,
                          const struct timespec e_time);

//dijikstra functions
int minDistance(int dist[], bool sptSet[]);
void printSolution(int dist[], int n);
void dijkstra(int graph[V][V], int src);
