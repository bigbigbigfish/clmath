#include <stdlib.h>

#include "clmath/hosts/vectors_cpu.h"

void vector_add_cpu (float * h_A,
                     float * h_B,
                     float * h_C,
                     float * h_D,
                     float * h_E,
                     float * h_F,
                     float * h_G,
                     const unsigned int count)
{
  for (int i = 0; i < count; ++i)
  {
    h_A[i] = (float)rand() / (float)RAND_MAX;
    h_B[i] = (float)rand() / (float)RAND_MAX;
    h_E[i] = (float)rand() / (float)RAND_MAX;
    h_G[i] = (float)rand() / (float)RAND_MAX;
  }

  for (int i = 0; i < count; ++i)
    h_F[i] = h_A[i] + h_B[i] + h_E[i] + h_G[i];
}


void vector_add_plus_cpu (float * h_A, 
                          float * h_B, 
                          float * h_C, 
                          float * h_D,
                          const unsigned int count)
{
  int i;

  for (i = 0; i < count; ++i)
  {
    h_A[i] = (float)rand() / (float)RAND_MAX;
    h_B[i] = (float)rand() / (float)RAND_MAX;
    h_C[i] = (float)rand() / (float)RAND_MAX;
  }

  for (i = 0; i < count; ++i)
    h_D[i] = h_A[i] + h_B[i] + h_C[i];
}
