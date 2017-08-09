#ifndef MATRIX_GPU_H
#define MATRIX_GPU_H

#include "clmath/devices/cl_engines.h"


void matrix_mul_gpu (engine * t,
                     const size_t global,
                     const size_t local,
                     float * h_A,
                     float * h_B,
                     float * h_C,
                     const unsigned int M,
                     const unsigned int N,
                     const unsigned int K);

#endif 
