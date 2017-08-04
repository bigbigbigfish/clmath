#ifndef VECTORS_GPU_H
#define VECTORS_GPU_H

#include "clmath/devices/cl_engines.h"

void vector_add_gpu (
                     float * h_A,
                     float * h_B,
                     float * h_C,
                     float * h_D,
                     float * h_E,
                     float * h_F,
                     float * h_G,
                     const int count);


void vector_add_plus_gpu (
                          float * h_A,
                          float * h_B,
                          float * h_C,
                          float * h_D,
                          const int count);

#endif
