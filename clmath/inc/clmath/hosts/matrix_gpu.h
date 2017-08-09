#ifndef MATRIX_GPU_H
#define MATRIX_GPU_H

void matrix_mul_gpu (engine * t,
                     float * h_A,
                     float * h_B,
                     float * h_C,
                     const unsigned int M,
                     const unsigned int N,
                     const unsigned int K);

#endif 
