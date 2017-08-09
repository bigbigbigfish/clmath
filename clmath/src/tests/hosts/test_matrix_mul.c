#include <stdio.h>
#include <stdlib.h>

#include "clmath/utils/eval.h"
#include "clmath/utils/timer.h"
#include "clmath/utils/file_handler.h"
#include "clmath/devices/cl_engines.h"
#include "clmath/hosts/matrix_cpu.h"
#include "clmath/hosts/matrix_gpu.h"


void matrix_mul_gpu_set1 (engine * t, float * h_A, 
                                      float * h_B, 
                                      float * h_C_gpu, 
                                      const unsigned int M,
                                      const unsigned int N,
                                      const unsigned int K)
{
  // kernel options: 
  // - ['matrix_mul_global', 
  //    'matrix_mul_shared', 
  //    'matrix_mul_register',
  //    'matrix_mul_vector']
  engine_compute (t, "matrix_mul_vector");
  // if matrix_mul_global, matrix_mul_shared, matrix_mul_register, TS = 32
  const size_t TS = 32;
  // if matrix_mul_global, matrix_mul_shared, matrix_mul_vector, WPT = 1;
  // elif matrix_mul_register, WPT = 8;
  const size_t WPT = 1;
  // if matrix_mul_global, matrix_mul_shared, matrix_mul_register, WIDTH = 1
  // elif matrix_mul_vector, WIDTH = 4;
  const size_t WIDTH = 4;                     
  const size_t global[2] = {M/WIDTH, N/WPT};
  const size_t local[2] = {TS/WIDTH, TS/WPT};
  matrix_mul_gpu (t, global, local, h_A, h_B, h_C_gpu, M, N, K);
}


void matrix_mul_gpu_set2 (engine * t, float * h_A, 
                                      float * h_B, 
                                      float * h_C_gpu, 
                                      const unsigned int M,
                                      const unsigned int N,
                                      const unsigned int K)
{
  // kernel options: 
  // - ['matrix_mul_transpose'] 
  engine_compute (t, "matrix_mul_transpose");
  const size_t TSM = 64;
  const size_t TSN = 64;
  const size_t WPTN = 1;
  const size_t global[2] = {M, N/WPTN};
  const size_t local[2] = {TSM, TSN/WPTN};
  matrix_mul_gpu (t, global, local, h_A, h_B, h_C_gpu, M, N, K);
}


int main (void)
{
  unsigned int M = 32*2;
  unsigned int N = 32*2;
  unsigned int K = 32*2;

  float * h_A = (float*)malloc(M*K*sizeof(float));
  float * h_B = (float*)malloc(K*N*sizeof(float));

  float * h_C_cpu = (float*)malloc(M*N*sizeof(float));
  float * h_C_gpu = (float*)malloc(M*N*sizeof(float));

  double tic_cpu = wtime(); 
  matrix_mul_cpu (h_A, h_B, h_C_cpu, M, N, K);
  double toc_cpu = wtime() - tic_cpu;


  engine * t = (engine*)malloc(sizeof(engine));
  char * kernel_srcs = file_read ("clmath/src/clmath/kernels/matrix_mul.cl");
  engine_init (t, kernel_srcs);

  double tic_gpu = wtime();
  matrix_mul_gpu_set1 (t, h_A, h_B, h_C_gpu, M, N, K);
  // matrix_mul_gpu_set2 (t, h_A, h_B, h_C_gpu, M, N, K);
  double toc_gpu = wtime() - tic_gpu;

  eval_results (h_C_cpu, h_C_gpu, M*N);
  printf ("CPU time: %f, GPU time: %f\n", toc_cpu, toc_gpu);

  // device
  engine_cleanup (t);
  // host
  free (h_A);
  free (h_B);
  free (h_C_cpu);
  free (h_C_gpu);

  return 0;
}
