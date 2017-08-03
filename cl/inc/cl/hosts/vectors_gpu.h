#ifndef VECTORS_GPU_H
#define VECTORS_GPU_H

#include "cl/devices/cl_engines.h"

void vector_add_plus_gpu (engine * t,
                          float * h_A,
                          float * h_B,
                          float * h_C,
                          float * h_D,
                          const unsigned int count);

#endif
