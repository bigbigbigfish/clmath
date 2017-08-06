#include <stdlib.h>

#include "clmath/hosts/matrix_cpu.h"
#include "clmath/hosts/matrix_gpu.h"


int main (void)
{
  unsigned int M = 24;
  unsigned int N = 24;
  unsigned int K = 24;

  float * h_A = (float*)malloc(M*K*sizeof(float));
  float * h_B = (float*)malloc(K*N*sizeof(float));

  float * h_C_cpu = (float*)malloc(M*N*sizeof(float));
  float * h_C_gpu = (float*)malloc(M*N*sizeof(float));

  matrix_mul_cpu (h_A, h_B, h_C_cpu, M, N, K);
  matrix_mul_gpu (h_A, h_B, h_C_gpu, M, N, K);
  // eval_results (h_C_cpu, h_C_gpu, M*N);

  free (h_A);
  free (h_B);
  free (h_C_cpu);
  free (h_C_gpu);

  return 0;
}
