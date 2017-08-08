#include <stdlib.h>

#include "clmath/utils/timer.h"
#include "clmath/hosts/matrix_cpu.h"
#include "clmath/hosts/matrix_gpu.h"


int main (void)
{
  unsigned int M = 32*1;
  unsigned int N = 32*1;
  unsigned int K = 32*1;

  float * h_A = (float*)malloc(M*K*sizeof(float));
  float * h_B = (float*)malloc(K*N*sizeof(float));

  float * h_C_cpu = (float*)malloc(M*N*sizeof(float));
  float * h_C_gpu = (float*)malloc(M*N*sizeof(float));

  double tic_cpu = wtime(); 
  matrix_mul_cpu (h_A, h_B, h_C_cpu, M, N, K);
  double toc_cpu = wtime() - tic_cpu;

  double tic_gpu = wtime();
  matrix_mul_gpu (h_A, h_B, h_C_gpu, M, N, K);
  double toc_gpu = wtime() - tic_gpu;

  eval_results (h_C_cpu, h_C_gpu, M*N);
  printf ("CPU time: %f, GPU time: %f\n", toc_cpu, toc_gpu);

  free (h_A);
  free (h_B);
  free (h_C_cpu);
  free (h_C_gpu);

  return 0;
}
