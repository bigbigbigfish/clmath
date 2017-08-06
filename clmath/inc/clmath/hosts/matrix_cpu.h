#ifndef MATRIX_CPU_H
#define MATRIX_CPU_H

void matrix_mul_cpu (float * h_A,
                     float * h_B,
                     float * h_C,
                     const unsigned int M,
                     const unsigned int N,
                     const unsigned int K);

#endif
