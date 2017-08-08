/*
 * Computation: D = A + B + C
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "clmath/utils/timer.h"
#include "clmath/utils/eval.h"
#include "clmath/hosts/vectors_cpu.h"
#include "clmath/hosts/vectors_gpu.h"


int main(void)
{
  size_t count = 1024;
  float * h_A = (float *)malloc(sizeof(float)*count);    
  float * h_B = (float *)malloc(sizeof(float)*count);     
  float * h_C = (float *)malloc(sizeof(float)*count);      
  float * h_D_cpu = (float *)malloc(sizeof(float)*count);    
  float * h_D_gpu = (float *)malloc(sizeof(float)*count);

  double tic_cpu = wtime();
  vector_add_plus_cpu (h_A, h_B, h_C, h_D_cpu, count);
  double toc_cpu = wtime() - tic_cpu;

  double tic_gpu = wtime();
  vector_add_plus_gpu (h_A, h_B, h_C, h_D_gpu, count);
  double toc_gpu = wtime();

  eval_results (h_D_cpu, h_D_gpu, count);
  printf ("CPU time: %f, GPU time: %f\n", toc_cpu, toc_gpu);

  free (h_A);
  free (h_B);
  free (h_C);
  free (h_D_cpu);
  free (h_D_gpu);

  return 0;
}

