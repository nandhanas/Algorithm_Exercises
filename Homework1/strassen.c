#include <stdlib.h>

#include "matrix.h"

void sum_matrix_blocks(float **C,
                       const size_t C_f_row,
                       const size_t C_f_col,
                       float ** A,
                       const size_t A_f_row,
                       const size_t A_f_col,
                       float ** B,
                       const size_t B_f_row,
                       const size_t B_f_col,
                       const size_t n)
{
  // for all the rows in the blocks
  for (size_t i=0; i<n; i++) {

    // for all the cols in the blocks
    for (size_t j=0; j<n; j++) {
      C[C_f_row+i][C_f_col+j] = A[A_f_row+i][A_f_col+j] +
                                B[B_f_row+i][B_f_col+j];
    }
  }
}

void sub_matrix_blocks(float **C,
                       const size_t C_f_row,
                       const size_t C_f_col,
                       float ** A,
                       const size_t A_f_row,
                       const size_t A_f_col,
                       float ** B,
                       const size_t B_f_row,
                       const size_t B_f_col,
                       const size_t n)
{
  // for all the rows in the blocks
  for (size_t i=0; i<n; i++) {

    // for all the cols in the blocks
    for (size_t j=0; j<n; j++) {
      C[C_f_row+i][C_f_col+j] = A[A_f_row+i][A_f_col+j] -
                                B[B_f_row+i][B_f_col+j];
    }
  }
}

void naive_aux(float **C, const size_t C_f_row, const size_t C_f_col,
               float **A, const size_t A_f_row, const size_t A_f_col,
               float **B, const size_t B_f_row, const size_t B_f_col,
               const size_t n)
{
   for (size_t i=0; i<n; i++) {
     for (size_t j=0; j<n; j++) {
       C[C_f_row+i][C_f_col+j] = 0.0;
       for (size_t k=0; k<n; k++) {
         C[C_f_row+i][C_f_col+j] += (A[A_f_row+i][A_f_col+k]*
                                     B[B_f_row+k][B_f_col+j]);
       }
     }

   }
}

void strassen_aux_opt(float **C, const size_t C_f_row, const size_t C_f_col,
                  float **A, const size_t A_f_row, const size_t A_f_col,
                  float **B, const size_t B_f_row, const size_t B_f_col,
 		              float **T, const size_t T_f_row, const size_t T_f_col,
                  const size_t n)

{
  if (n < (1<<6)) {
    naive_aux(C, C_f_row, C_f_col,
              A, A_f_row, A_f_col,
              B, B_f_row, B_f_col,
              n);
    return;
  }

  const size_t n2=n/2;

  const size_t C1X = C_f_row;
  const size_t C2X = C_f_row + n2;
  const size_t CX1 = C_f_col;
  const size_t CX2 = C_f_col + n2;

  const size_t A1X = A_f_row;
  const size_t A2X = A_f_row + n2;
  const size_t AX1 = A_f_col;
  const size_t AX2 = A_f_col + n2;

  const size_t B1X = B_f_row;
  const size_t B2X = B_f_row + n2;
  const size_t BX1 = B_f_col;
  const size_t BX2 = B_f_col + n2;

  //Auxilary Matrix 
  const size_t T1X = T_f_row;
  const size_t T2X = T_f_row + n2;
  const size_t TX1 = T_f_col;
  const size_t TX2 = T_f_col + n2;

  /// C11 = (P5 + P4) - P2 + P6
  	sub_matrix_blocks(C, C2X, CX2, //S7
        	            A,A1X,AX2,
        	            A,A2X,AX2,n2);

  	sum_matrix_blocks(C, C1X, CX2, //S8
        	            B,B2X,BX1,
        	            B,B2X,BX2,n2);

  	strassen_aux_opt(T, T1X, TX1, //P6
        	           C, C2X, CX2, //S7
        	           C, C1X, CX2, //S8
		                 T,T2X,TX2,n2);

  // compute P2
  	// P2 = S2 x B22
  	// where S2 = A11 + A12
 	 sum_matrix_blocks(C, C2X, CX2, //S2
 	                   A,A1X,AX1,
 	                   A,A1X,AX2,n2);

 	 strassen_aux_opt(C, C1X, CX2, //P2
  	                C, C2X, CX2, //S2
  	                B, B2X, BX2,
		                T,T2X,TX2,n2);

  // compute P5
  	// P5 = S5 x S6
  	// where S5 = A11 + A22 and S6 = B11 + B22
  	sum_matrix_blocks(C, C2X, CX2, //S5
  	                  A,A1X,AX1,
  	                  A,A2X,AX2,n2);


  	sum_matrix_blocks(C, C2X, CX1, //S6
        	            B,B1X,BX1,
        	            B,B2X,BX2,n2);

  	strassen_aux_opt(T, T1X, TX2, // P5
		                 C, C2X, CX2, //S5
        	           C, C2X, CX1, //S6
		                 T,T2X,TX2,n2);

  //compute P4
  	// P4 = A22 x S4
  	// where S4 = B21 - B11
  	sub_matrix_blocks(C, C2X, CX2, //S4
        	            B,B2X,BX1,
        	            B,B1X,BX1,n2);

  	strassen_aux_opt(T, T2X, TX1, // P4
        	           A, A2X, AX2,
        	           C, C2X, CX2, //S4
		                 T,T2X,TX2,n2);

  // P4 + P5
  sum_matrix_blocks(C, C1X, CX1,
  	                T, T1X, TX2, //P5
  	                T, T2X, TX1, //P4
  	                n2);

  // (P4 + P5) - P2
  sub_matrix_blocks(C, C1X, CX1,
                    C, C1X, CX1,
                    C, C1X, CX2, //P2
                    n2);

  // ((P4 + P5) - P2) + P6
  sum_matrix_blocks(C, C1X, CX1,
                    C, C1X, CX1,
                    T, T1X, TX1, //P6
                    n2);



  /// C12 = P1 + P2

  // compute P1
 	 // P1 = A11 x S1
  	 // where S1 = B12 - B22
  sub_matrix_blocks(C, C2X, CX1, //S1
        	            B, B1X,BX2,
        	            B, B2X,BX2,n2);

	strassen_aux_opt(C, C2X, CX2, //P1
	                 A, A1X, AX1,
	                 C, C2X, CX1, //S1
		               T, T2X, TX2,n2);

  sum_matrix_blocks(C, C1X, CX2,
                  C, C2X, CX2, //P1
                  C, C1X, CX2, //P2
                  n2);


  // C21 = P3 + P4

  // compute P3
 	 // P3 = S3 x B11
 	 // where S3 = A21 + A22
 	 sum_matrix_blocks(C, C2X, CX1, //S3
  	                 A, A2X, AX1,
  	                 A, A2X, AX2,n2);

  	strassen_aux_opt(T, T1X, TX1, //P3
        	                C, C2X, CX1, //S3
        	                B, B1X, BX1,
		                      T,T2X,TX2,n2);

    sum_matrix_blocks(C, C2X, CX1,
                      T, T1X, TX1, //P3
                      T, T2X, TX1, //P4
                      n2);

  // C22 = P5 + P1 - P3 - P7

  // P1 + P5
  sum_matrix_blocks(C, C2X, CX2,
                    C, C2X, CX2, //P1
                    T, T1X, TX2, //P5
                    n2);

  // (P5 + P1) - P3
  sub_matrix_blocks(C, C2X, CX2,
                    C, C2X, CX2,
                    T, T1X, TX1, //P3
                    n2);

  //compute P7
  	// P7 = S9 x S10
  	// where S9 = A11 - A21 and S10 = B11 + B12
 	 sub_matrix_blocks( T, T1X, TX1, //S9
        	            A,A1X,AX1,
        	            A,A2X,AX1,n2);

  	 sum_matrix_blocks( T, T1X, TX2, //S10
        	            B,B1X,BX1,
        	            B,B1X,BX2,n2);

  	 strassen_aux_opt(T, T2X, TX1, //P7
        	      T, T1X, TX1, //S9
        	      T, T1X, TX2, //S10
		            T,T2X,TX2,n2);

  // ((P5 + P1) - P3) - P7
  sub_matrix_blocks(C, C2X, CX2,
                    C, C2X, CX2,
                    T, T2X, TX1, //P7
                    n2);

}
void strassen(float **C,
      float **A, float **B, const size_t n)
{
  float **T = allocate_matrix(n,n);
  strassen_aux_opt(C, 0, 0, A, 0, 0, B, 0, 0, T, 0, 0, n);
  deallocate_matrix(T,n);
}
