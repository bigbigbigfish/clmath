#include <stdlib.h>

#include "clmath/hosts/matrix_cpu.h"


void matrix_mul_cpu (float * h_A,
                     float * h_B,
                     float * h_C,
                     const unsigned int M,
                     const unsigned int N,
                     const unsigned int K)
{
  float tmp;

  for (int i = 0; i < M*K; ++i)
    h_A[i] = (float)rand() / (float)RAND_MAX;

  for (int i = 0; i < K*N; ++i)
    h_B[i] = (float)rand() / (float)RAND_MAX;


  for (unsigned int i = 0; i < M; ++i)
  {
    for (unsigned int j = 0; j < N; ++j)
    {
      tmp = 0.0f;
      for (unsigned int k = 0; k < K; ++k)
      {
        tmp += h_A[i*K+k] * h_B[k*K+j];
      }
      h_C[i*K+j] = tmp;
    }
  }
}
