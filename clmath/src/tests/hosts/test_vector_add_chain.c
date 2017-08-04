#include <stdio.h>
#include <stdlib.h>

#include "clmath/hosts/vectors_gpu.h"


// extern int output_device_info(cl_device_id );


int main (void)
{

  size_t count = 1024;
  float * h_A = (float *)malloc(sizeof(float)*count);       // a vector
  float * h_B = (float *)malloc(sizeof(float)*count);       // b vector
  float * h_C = (float *)malloc(sizeof(float)*count);       // c vector (result)
  float * h_D = (float *)malloc(sizeof(float)*count);       // d vector (result)
  float * h_E = (float *)malloc(sizeof(float)*count);       // e vector
  float * h_F_cpu = (float *)malloc(sizeof(float)*count);   // f vector (result)
  float * h_F_gpu = (float *)malloc(sizeof(float)*count);   // f vector (result)
  float * h_G = (float *)malloc(sizeof(float)*count);       // g vector

  vector_add_cpu (h_A, h_B, h_C, h_D, h_E, h_F_cpu, h_G, count);
  vector_add_gpu (h_A, h_B, h_C, h_D, h_E, h_F_gpu, h_G, count);    
  eval_results (h_F_cpu, h_F_gpu, count);
    
  free (h_A);
  free (h_B);
  free (h_C);
  free (h_D);
  free (h_E);
  free (h_F_cpu);
  free (h_F_gpu);
  free (h_G);

  return 0;
}
