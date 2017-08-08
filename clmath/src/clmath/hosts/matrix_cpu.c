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

  for (unsigned int m = 0; m < M; ++m)
  {
    for (unsigned int n = 0; n < N; ++n)
    {
      float acc = 0.0f;
      for (unsigned int k = 0; k < K; ++k)
      {
        acc += h_A[k*M + m] * h_B[n*K + k];
      }
      h_C[n*M + m] = acc;
    }
  }
}
