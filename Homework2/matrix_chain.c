#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "matrix.h"

#define MAX_VALUE 100  // maximum possible dimension
#define N_REPETITIONS 3
//  'm' and 's' matrices of the algorithm are half-filled, so we can half fill by compacting rows to the left.

#define IND(mat, row, col) mat[row][col - row] //compacted index


/**
  * This function will perform parenthetization.It takes pointer to array of dimensions and its size n .
  */
size_t** matrix_chain(size_t* dim, size_t n) {

    // allocation for matrices
    size_t** m = (size_t**)malloc(sizeof(size_t*) * n);
    size_t** s = (size_t**)malloc(sizeof(size_t*) * n);

    for (size_t i = 0; i < n; ++i) {
        // initializing  rows
        m[i] = (size_t*)malloc(sizeof(size_t) * (n - i));
        s[i] = (size_t*)malloc(sizeof(size_t) * (n - i));

        //Main diagonal with cost 0.
        IND(m, i, i) = 0;
        // The index of matrix itself goes into the s table as it is the chain of itself.
        IND(s, i, i) = i;
    }
    // for each diagonal
    for (size_t d= 1; d< n; ++d) {
        // for each row of the diagonal
        for (size_t i = 0; i < n - d; ++i) {
            size_t j = i+d;  // column index
            IND(m, i, j) = IND(m, i, j - 1) + IND(m, j, j) + dim[i] * dim[j + 1] * dim[j];
            IND(s, i, j) = j - 1;

            //  dynamic programming section to find the minimum cost

            for (size_t k = i + 1; k < j; ++k) {
                size_t q = IND(m, i, k) + IND(m, k + 1, j) + dim[i] * dim[j + 1] * dim[k + 1];
                if (q < IND(m, i, j)) {
                    IND(m, i, j) = q;
                    IND(s, i, j) = k;
                }
            }
        }
    }
    // deallocation of the `m` matrix  ( deallocate_matrix  not used here because of different row size) ,
    for (size_t i = 0; i < n; i++) {
        free(m[i]);
    }
    free(m);
    return s;
}

/**
  * This function  copies an input matrix 'AX' with 'r' rows
  * and 'c' columns.
  */
float** copy_matrix(float** AX, size_t r, size_t c) {
    float** res = allocate_matrix(r, c);
    for (size_t i=0; i < r; ++i) {
        for (size_t j=0; j < c; ++j) {
            res[i][j] = AX[i][j];
        }
    }
    return res;
}

/** Allocate and return an array of matrices with dimensions
  * given by 'dims` and size  'n'
  */
float*** build_problem_instance(size_t *dims, const size_t n) {
    float*** A = (float***)malloc(sizeof(float **) * n);
    for (size_t i=0; i < n; ++i) {
        A[i] = allocate_matrix(dims[i], dims[i + 1]);
    }
    return A;
}

/** Return an array of dimensions, choosen randomly upto n values.
  */
size_t* build_dimensions(const size_t n) {
    size_t* dims = (size_t*)malloc(sizeof(size_t) * (n + 1));
    for (size_t i=0; i < n + 1; ++i) {
        dims[i] = rand() % MAX_VALUE;  // Random value limited by MAX_VALUE
    }
    return dims;
}

/**
 * This function recursively computes matrix multiplication between different AX with dimension dims and length n .
 * S indicates where splitting is done or parenthesis can be inserted in optimal way and `i` and `j `indicates
 *which subchain of matrices to select.
  */
float** compute_matrix_chain_aux(float ***AX, size_t *dims, const size_t n, size_t **S, size_t i, size_t j) {
    // if only one matrix,  return it by calling copy matrix routine.
    if (i == j) {
        return copy_matrix(AX[i], dims[i], dims[j + 1]);
    }
    // indicates the parenthesis with low cost ,  i j
    size_t p = IND(S, i, j);

    // low cost parenthesis for subchain i, ..., p
    float** C1 = compute_matrix_chain_aux(AX, dims, n, S, i, p);

    //  low cost parenthesis for subchain p + 1, ..., j
    float** C2 = compute_matrix_chain_aux(AX, dims, n, S, p + 1, j);

    // allocating matrix for results with dimension (i,j+1)
    float**R  = allocate_matrix(dims[i], dims[j + 1]);

    // performing naive matrix multiplication
    naive_matrix_mult(R, C1, C2, dims[i], dims[p + 1], dims[p + 1], dims[j + 1]);

    deallocate_matrix(C1, dims[i]);
    deallocate_matrix(C2, dims[p + 1]);
    return R;
}

/**
  *This function computes the matrix product  AX with parenthesis determined by S.
  */
float** compute_matrix_chain(float ***AX, size_t *dims, const size_t n, size_t **S) {
    // return the main routine, with subchain from 0 to n - 1
    return compute_matrix_chain_aux(AX, dims, n, S, 0, n - 1);
}

/**
  * This function uses naive matrix multiplication that simply multiplies matrices successively.
  */
float** compute_naive_matrix_chain(float ***AX, size_t *dims, const size_t n) {
  float** C = copy_matrix(AX[0], dims[0], dims[1]);
  float** R;
  for (size_t i = 1; i < n; i++) {
      R= allocate_matrix(dims[0], dims[i + 1]);
      naive_matrix_mult(R, C, AX[i], dims[0], dims[i], dims[i], dims[i + 1]);
      deallocate_matrix(C, dims[0]);
      C = R;
  }
  return C;
}

/**
  * This function returns the number of second elapsed .
  */
double seconds() {
    struct timeval tmp;
    double sec;
    gettimeofday(&tmp, (struct timezone*)0);
    sec = tmp.tv_sec + ((double)tmp.tv_usec) / 1000000.0;
    return sec;
}

int main() {
  // random seed
  srand(123);
  //number of total matrices
  size_t n = 10;
  // initialize the dimensions
  size_t* dims = build_dimensions(n);
  // allocate matrices
  float*** AX = build_problem_instance(dims, n);
  // time facilities
  double acc;
  printf("N \t Optimal(s) \tNaive(s) \n");


  for (size_t d = 1; d < n; d++) {
      printf("%ld", d);
      double start = seconds();
      // evaluate performance for dynamic programming algorithm
      for (int r = 0; r < N_REPETITIONS; r++) {
          size_t** S = matrix_chain(dims, d);
          float ** R= compute_matrix_chain(AX, dims, d, S);
          deallocate_matrix(R, dims[0]);
          deallocate_matrix((float**)S, d);
      }
      double end = seconds();

      acc = end - start;
      printf("\t %lf", (acc / N_REPETITIONS));
      start = seconds();

      // evaluate performance for the naive algorithm

     for (int r = 0; r < N_REPETITIONS; ++r) {
          float **R = compute_naive_matrix_chain(AX, dims, d);
          deallocate_matrix(R, dims[0]);
      }
      end = seconds();
      acc = end - start;
      printf("\t%lf\n", (acc / N_REPETITIONS));
  }

  // deallocate
  for (size_t i = 0; i < n; i++) {
      deallocate_matrix(AX[i], dims[i]);
  }
  free(AX);
  free(dims);
  return 0;
}
